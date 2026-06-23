## UTILITIES
import numpy as np 
import time  

# from numba import jit
##@jit(nopython=True)
def grad(S, X, bc='e', order=2):
    """
    Compute gradient of scalar field S on a rectilinear grid.
    
    Parameters:
    -----------
    S : ndarray
        Scalar field values on the grid. Shape should match the grid dimensions.
    X : list of ndarray
        List of 1D coordinate arrays for each dimension [x, y, z, ...]
    bc : str or list of str
        Boundary condition for each dimension: 'e' (exact), 'n' (Neumann), 'd' (Dirichlet)
        Can be single string (same for all dims) or list per dimension
    order : int or list of int
        Derivative order (2 or 4) for each dimension
    
    Returns:
    --------
    dS : list of ndarray
        List of gradient components, each same shape as S
        dS[0] = dS/dx, dS[1] = dS/dy, etc.
    """
    
    # Input validation
    ndim = len(X)
    
    # Handle bc and order as single values or lists
    if isinstance(bc, str):
        bc = [bc] * ndim
    if isinstance(order, int):
        order = [order] * ndim
    
    # Check S shape matches grid dimensions
    expected_shape = tuple(len(x) for x in X)
    if S.shape != expected_shape:
        raise ValueError(f"S shape {S.shape} doesn't match grid {expected_shape}")
    
    # Initialize gradient list
    dS = []
    
    # Compute gradient for each dimension
    for dim in range(ndim):
        # Get derivative matrix for this dimension
        D = Df(X[dim], order=order[dim], bc=bc[dim])
        
        # Move dimension 'dim' to front
        axes_roll = list(range(ndim))
        axes_roll.remove(dim)
        axes_roll = [dim] + axes_roll
        
        # Roll S so target dimension is first
        S_rolled = np.transpose(S, axes_roll)
        
        # Reshape to 2D: (N_dim, N_rest)
        N_dim = len(X[dim])
        N_rest = int(np.prod(S_rolled.shape[1:]))  # FIXED: Convert to int
        S_2d = S_rolled.reshape(N_dim, N_rest)
        
        # Apply derivative matrix
        dS_2d = D @ S_2d
        
        # Reshape back
        dS_rolled = dS_2d.reshape(S_rolled.shape)
        
        # Roll back to original orientation
        inverse_axes = np.argsort(axes_roll)
        dS_component = np.transpose(dS_rolled, inverse_axes)
        
        dS.append(dS_component)
    
    return dS

##@jit(nopython=True)
def divergence(V, X, bc='e', order=2):
    """
    Compute divergence of vector field V on a rectilinear grid.
    
    Parameters:
    -----------
    V : list of ndarray
        List of vector components [Vx, Vy, Vz, ...], each same shape
    X : list of ndarray
        List of 1D coordinate arrays for each dimension
    bc : str or list of str
        Boundary condition for each dimension
    order : int or list of int
        Derivative order for each dimension
    
    Returns:
    --------
    divV : ndarray
        Divergence field ∇·V, same shape as components
    """
    
    ndim = len(V)
    divV = np.zeros_like(V[0])
    
    for dim in range(ndim):
        # Get derivative of V[dim] with respect to X[dim]
        dV_dim = grad(V[dim], X, bc=bc, order=order)[dim]
        divV += dV_dim
    
    return divV

##@jit(nopython=True)
def laplacian(S, X, bc='e', order=2):
    """
    Compute Laplacian of scalar field S on a rectilinear grid.
    
    Parameters:
    -----------
    S : ndarray
        Scalar field
    X : list of ndarray
        List of 1D coordinate arrays
    bc : str or list of str
        Boundary condition for Laplacian (applies to second derivative)
    order : int or list of int
        Derivative order
    
    Returns:
    --------
    lapS : ndarray
        Laplacian ∇²S, same shape as S
    """
    
    ndim = len(X)
    lapS = np.zeros_like(S)
    
    # Handle bc/order as lists
    if isinstance(bc, str):
        bc_list = [bc] * ndim
    else:
        bc_list = bc
    if isinstance(order, int):
        order_list = [order] * ndim
    else:
        order_list = order
    
    for dim in range(ndim):
        # Second derivative along dimension dim
        D2 = DD(X[dim], bc=bc_list[dim])
        
        # Roll dimensions
        axes_roll = list(range(ndim))
        axes_roll.remove(dim)
        axes_roll = [dim] + axes_roll
        
        S_rolled = np.transpose(S, axes_roll)
        N_dim = len(X[dim])
        N_rest = int(np.prod(S_rolled.shape[1:]))  # FIXED: Convert to int
        S_2d = S_rolled.reshape(N_dim, N_rest)
        
        d2S_2d = D2 @ S_2d
        d2S_rolled = d2S_2d.reshape(S_rolled.shape)
        
        inverse_axes = np.argsort(axes_roll)
        d2S = np.transpose(d2S_rolled, inverse_axes)
        
        lapS += d2S
    
    return lapS


# ============================================
# Supporting derivative matrix functions
# ============================================
##@jit(nopython=True)
def Df(x, order=2, bc='e'):
    """
    First derivative matrix for 1D grid.
    
    Parameters:
    -----------
    x : ndarray
        1D coordinate array
    order : int
        2 or 4 for 2nd/4th order accuracy
    bc : str
        'e' (exact/one-sided), 'n' (Neumann), 'd' (Dirichlet)
    
    Returns:
    --------
    D : ndarray
        Derivative matrix of shape (N, N)
    """
    N = len(x)
    dx = x[1] - x[0]
    D = np.zeros((N, N))
    
    if order == 2:
        # Interior points: central difference
        for i in range(1, N-1):
            D[i, i-1] = -1
            D[i, i+1] = 1
        
        # Boundaries
        if bc == 'e':
            # One-sided 2nd order
            D[0, 0:3] = [-3, 4, -1]
            D[-1, -3:] = [1, -4, 3]
        elif bc == 'n':
            # Neumann: derivative zero
            D[0, 0] = 0
            D[-1, -1] = 0
        elif bc == 'd':
            # Dirichlet: value zero (for derivative, this means no constraint)
            # Use one-sided but with zeros? Better to just use exact
            D[0, 0:3] = [-3, 4, -1]
            D[-1, -3:] = [1, -4, 3]
        else:
            raise ValueError(f"Unknown bc '{bc}'")
        
        D /= (2*dx)
    
    elif order == 4:
        # Interior points: 4th order central
        for i in range(2, N-2):
            D[i, i-2] = 1
            D[i, i-1] = -8
            D[i, i+1] = 8
            D[i, i+2] = -1
        
        # Boundaries: 4th order one-sided
        if bc == 'e':
            D[0, 0:5] = [-25, 48, -36, 16, -3]
            D[1, 0:5] = [-3, -10, 18, -6, 1]
            D[-2, -5:] = [-1, 6, -18, 10, 3]
            D[-1, -5:] = [3, -16, 36, -48, 25]
        elif bc == 'n':
            D[0, :] = 0
            D[1, :] = 0
            D[-2, :] = 0
            D[-1, :] = 0
        elif bc == 'd':
            D[0, 0:5] = [-25, 48, -36, 16, -3]
            D[1, 0:5] = [-3, -10, 18, -6, 1]
            D[-2, -5:] = [-1, 6, -18, 10, 3]
            D[-1, -5:] = [3, -16, 36, -48, 25]
        else:
            raise ValueError(f"Unknown bc '{bc}'")
        
        D /= (12*dx)
    
    else:
        raise ValueError(f"Order must be 2 or 4, got {order}")
    
    return D

##@jit(nopython=True)
def DD(x, bc='d'):
    """
    Second derivative matrix for 1D grid.
    
    Parameters:
    -----------
    x : ndarray
        1D coordinate array
    bc : str
        'd' (Dirichlet: u=0), 'n' (Neumann: du/dx=0), 
        'p' (periodic), 'e' (exact/one-sided)
    
    Returns:
    --------
    D2 : ndarray
        Second derivative matrix of shape (N, N)
    """
    N = len(x)
    dx = x[1] - x[0]
    D2 = np.zeros((N, N))
    
    # Interior: standard 2nd order
    for i in range(1, N-1):
        D2[i, i-1] = 1
        D2[i, i] = -2
        D2[i, i+1] = 1
    
    # Boundaries
    if bc == 'n':  # Neumann (du/dx=0)
        D2[0, 0:2] = [-2, 2]
        D2[-1, -2:] = [2, -2]
    elif bc == 'd':  # Dirichlet (u=0)
        D2[0, :] = 0
        D2[-1, :] = 0
        D2[0, 0] = 1  # Identity to enforce u=0
        D2[-1, -1] = 1
    elif bc == 'p':  # Periodic
        D2[0, 0] = -2
        D2[0, 1] = 1
        D2[0, -1] = 1
        D2[-1, -1] = -2
        D2[-1, -2] = 1
        D2[-1, 0] = 1
    elif bc == 'e':  # Exact (one-sided)
        D2[0, 0:4] = [2, -5, 4, -1]
        D2[-1, -4:] = [2, -5, 4, -1][::-1]
    else:
        raise ValueError(f"Unknown bc '{bc}'")
    
    D2 /= dx**2
    return D2

##@jit(nopython=True)
def curl(V, X, bc='e', order=2):
    """
    Compute curl of a 2D or 3D vector field on a rectilinear grid.
    
    Parameters:
    -----------
    V : list of ndarray
        List of vector components [Vx, Vy, Vz, ...]
        For 2D: [Vx, Vy] (returns scalar curl in z-direction)
        For 3D: [Vx, Vy, Vz] (returns vector curl [curl_x, curl_y, curl_z])
    X : list of ndarray
        List of 1D coordinate arrays for each dimension [x, y, z, ...]
    bc : str or list of str
        Boundary condition for each dimension
    order : int or list of int
        Derivative order for each dimension
    
    Returns:
    --------
    curlV : ndarray or list of ndarray
        2D: scalar field of z-component of curl (∂Vy/∂x - ∂Vx/∂y)
        3D: list of three components [curl_x, curl_y, curl_z]
    """
    
    ndim = len(V)
    
    if ndim == 2:
        # 2D curl: only z-component (scalar)
        # curl_z = ∂Vy/∂x - ∂Vx/∂y
        
        # Compute ∂Vy/∂x
        dVy_dx = grad(V[1], X, bc=bc, order=order)[0]
        
        # Compute ∂Vx/∂y
        dVx_dy = grad(V[0], X, bc=bc, order=order)[1]
        
        # Curl in 2D (scalar)
        curl_z = dVy_dx - dVx_dy
        
        return curl_z
    
    elif ndim == 3:
        # 3D curl: vector field
        # curl_x = ∂Vz/∂y - ∂Vy/∂z
        # curl_y = ∂Vx/∂z - ∂Vz/∂x
        # curl_z = ∂Vy/∂x - ∂Vx/∂y
        
        # Get all necessary derivatives
        dVz_dy = grad(V[2], X, bc=bc, order=order)[1]  # ∂Vz/∂y
        dVy_dz = grad(V[1], X, bc=bc, order=order)[2]  # ∂Vy/∂z
        dVx_dz = grad(V[0], X, bc=bc, order=order)[2]  # ∂Vx/∂z
        dVz_dx = grad(V[2], X, bc=bc, order=order)[0]  # ∂Vz/∂x
        dVy_dx = grad(V[1], X, bc=bc, order=order)[0]  # ∂Vy/∂x
        dVx_dy = grad(V[0], X, bc=bc, order=order)[1]  # ∂Vx/∂y
        
        # Curl components
        curl_x = dVz_dy - dVy_dz
        curl_y = dVx_dz - dVz_dx
        curl_z = dVy_dx - dVx_dy
        
        return [curl_x, curl_y, curl_z]
    
    else:
        raise ValueError(f"Curl only defined for 2D or 3D, got {ndim}D")

##@jit(nopython=True)
def vector_potential(V, X, bc='e', order=2):
    """
    Compute vector potential A such that curl(A) = V (for divergence-free fields in 3D).
    For 2D, returns stream function ψ such that V = (∂ψ/∂y, -∂ψ/∂x).
    
    Parameters:
    -----------
    V : list of ndarray
        Vector field components [Vx, Vy] for 2D or [Vx, Vy, Vz] for 3D
    X : list of ndarray
        List of 1D coordinate arrays
    bc : str or list of str
        Boundary condition
    order : int or list of int
        Derivative order
    
    Returns:
    --------
    A : ndarray or list of ndarray
        2D: stream function ψ (scalar)
        3D: vector potential [Ax, Ay, Az]
    """
    
    ndim = len(V)
    
    if ndim == 2:
        # For 2D, find stream function ψ such that:
        # Vx = ∂ψ/∂y, Vy = -∂ψ/∂x
        
        # Integrate Vx with respect to y
        # This is simplified - a full implementation would need careful integration
        from scipy import integrate
        
        # Placeholder - proper 2D stream function computation is nontrivial
        raise NotImplementedError("2D vector potential (stream function) requires careful integration")
    
    elif ndim == 3:
        # For 3D divergence-free field, find A such that curl(A) = V
        # This uses the Coulomb gauge ∇·A = 0
        # A = ∫ (V × r̂) dV type integral - simplified version
        
        # Placeholder for full implementation
        raise NotImplementedError("3D vector potential requires Poisson solvers")
    
    else:
        raise ValueError(f"Vector potential only defined for 2D or 3D, got {ndim}D")


# ============================================
# Additional vector calculus identities
# ============================================
##@jit(nopython=True)
def vorticity(V, X, bc='e', order=2):
    """
    Compute vorticity (curl of velocity field).
    Alias for curl for 2D/3D flows.
    
    Parameters:
    -----------
    V : list of ndarray
        Velocity field components
    X : list of ndarray
        Coordinate arrays
    bc : str or list of str
        Boundary conditions
    order : int or list of int
        Derivative order
    
    Returns:
    --------
    ω : ndarray or list
        Vorticity field
    """
    return curl(V, X, bc=bc, order=order)

##@jit(nopython=True)
def grad_div(V, X, bc='e', order=2):
    """
    Compute gradient of divergence: ∇(∇·V)
    
    Parameters:
    -----------
    V : list of ndarray
        Vector field components
    X : list of ndarray
        Coordinate arrays
    bc : str or list of str
        Boundary conditions
    order : int or list of int
        Derivative order
    
    Returns:
    --------
    grad_divV : list of ndarray
        Gradient of divergence field
    """
    divV = divergence(V, X, bc=bc, order=order)
    return grad(divV, X, bc=bc, order=order)

##@jit(nopython=True)
def laplacian_vector(V, X, bc='e', order=2):
    """
    Compute vector Laplacian: ∇²V = (∇²Vx, ∇²Vy, ∇²Vz)
    
    Parameters:
    -----------
    V : list of ndarray
        Vector field components
    X : list of ndarray
        Coordinate arrays
    bc : str or list of str
        Boundary conditions
    order : int or list of int
        Derivative order
    
    Returns:
    --------
    lapV : list of ndarray
        Vector Laplacian components
    """
    lapV = []
    for component in V:
        lapV.append(laplacian(component, X, bc=bc, order=order))
    return lapV

GREEN = '\033[92m'
RESET = '\033[0m'
# Custom progress bar function
def format_time(seconds, ms=False):
    if seconds < 3600:
        if ms:
            minutes = int(seconds // 60)
            secs = int(seconds % 60)
            millis = int((seconds % 1) * 1000)
            return f"{minutes:02d}:{secs:02d}.{millis:03d}"
        else:
            return f"{int(seconds // 60):02d}:{int(seconds % 60):02d}"
    else:
        hours = int(seconds // 3600)
        minutes = int((seconds % 3600) // 60)
        secs = int(seconds % 60)
        if ms:
            millis = int((seconds % 1) * 1000)
            return f"{hours:02d}:{minutes:02d}:{secs:02d}.{millis:03d}"
        else:
            return f"{hours:02d}:{minutes:02d}:{secs:02d}"

def update_progress(current, total, start_time, bar_width=60):
    """Display a custom progress bar"""
    percent = current / total
    percent_int = int(percent * 100)
    
    # Calculate elapsed time
    elapsed = time.time() - start_time
    
    # Estimate remaining time
    if current > 0:
        time_per_frame = elapsed / current
        remaining = time_per_frame * (total - current)
        eta_str = format_time(remaining)
    else:
        eta_str = "??:??"
    
    # Create the bar
    filled = int(bar_width * percent)
    bar = f"{GREEN}{'▬' * filled}{RESET}{'▬' * (bar_width - filled)}"
    
    # Format elapsed time
    elapsed_str = format_time(elapsed)
    
    # Build progress string
    progress_str = f"\r[{bar}] {current:4d}/{total} | {percent_int:3d}% | TE: {elapsed_str} | ETA: {eta_str}"
    
    # Print without newline
    print(progress_str, end='', flush=True)

# ===========================================================
# SIM START =================================================
# ===========================================================
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter
import os
import shutil
import time
from numba import jit

time_start = time.time()

## Use computer modern 
plt.rcParams.update({
    "text.usetex": True,
    "font.family": "serif",
    "font.size": 9
})
print("="*100)
print(f"{"TRANSPORT EQUATION SIMULATION":^100}")
print("="*100)
# Create "temp" directory (delete if exists)
print("Creating temporary directory...")
temp_dir = "temp"
if os.path.exists(temp_dir):
    shutil.rmtree(temp_dir)
os.makedirs(temp_dir)

# Grid setup
N = 100
x = np.linspace(-4, 4, N)
y = np.linspace(-4, 4, N)
xv, yv = np.meshgrid(x, y, indexing='ij')

# Initial density (Gaussian blob)
p0 = [1, 0]  # Center at x=1, y=0
r = np.sqrt((xv - p0[0])**2 + (yv - p0[1])**2)
rho = np.exp(-r**2)
X = [x, y]
V = np.zeros((2, len(x), len(y)))

# # Velocity field: v = (1, 0) everywhere

# V[0] = np.ones((len(x), len(y))) * 4   # vx = 4
# V[1] = np.zeros((len(x), len(y)))      # vy = 0

# Velocity field: rotating, constant magnitude 1
# For a point at (x,y), the tangent direction (clockwise) is (y, -x)
# Normalize to magnitude 1
# # Calculate radius
# r = np.sqrt(xv**2 + yv**2)

# # Clockwise rotation: v = (y/r, -x/r)
# # Handle r=0 to avoid division by zero
# r_safe = np.where(r == 0, 1, r)  # Avoid division by zero at center

# V[0] = yv / r_safe   # vx = y/r  (clockwise rotation)
# V[1] = -xv / r_safe  # vy = -x/r

# # Set center point (r=0) to zero
# V[0] = np.where(r == 0, 0, V[0])
# V[1] = np.where(r == 0, 0, V[1])

# Keplerian rotation (like planets around a central mass)
# v = sqrt(GM/r), set GM=1 for simplicity
# r = np.sqrt(xv**2 + yv**2)
# r_safe = np.where(r == 0, 1, r)

# v_mag = 1.0 / np.sqrt(r_safe)  # v = 1/√r

# # Clockwise tangential direction
# V[0] = v_mag * yv / r_safe     # vx = -v * y/r
# V[1] = - v_mag * xv / r_safe   # vy = v * x/r

# # Set center to zero
# V[0] = np.where(r == 0, 0, V[0])
# V[1] = np.where(r == 0, 0, V[1])

S = np.cos(xv) * np.sin(yv)
V = 2 * grad(S,X)

# Central velocity (like planets around a central mass)
# v = sqrt(GM/r), set GM=1 for simplicity
# r = np.sqrt(xv**2 + yv**2)
# r_safe = np.where(r == 0, 1, r)

# v_mag = 1.0 / np.sqrt(r_safe)  # v = 1/r (or 1/sqrt(r) depending on what you want)

# # Velocity toward center: V = -v_mag * (r_hat)
# Where r_hat = (x/r, y/r)
# V[0] = -v_mag * xv / r_safe  # vx = -v * x/r
# V[1] = -v_mag * yv / r_safe  # vy = -v * y/r

# # Set center to zero
# V[0] = np.where(r == 0, 0, V[0])
# V[1] = np.where(r == 0, 0, V[1])
 
# Time parameters
dt = 0.01
total_time = 15
t = np.arange(0,total_time+dt,dt)
num_steps = int(total_time / dt)

# Storage for mass conservation check
initial_mass = np.sum(rho)


frames = 0
total_frames = int( total_time / (dt * 5)) + 1

##@jit(nopython=True)
def drho_dt(rho):
    # grad_rho = grad(rho, X, bc='n', order=2)
    # div_v = divergence(V, X, bc='n', order=2)
    # Zero divergence at the boundary
    # div_v[0, :] = 0
    # div_v[-1, :] = 0
    # div_v[:, 0] = 0
    # div_v[:, -1] = 0
    # grad_rho_dot_v = grad_rho[0] * V[0] + grad_rho[1] * V[1]
    rhoV = np.array([rho * V[0],rho * V[1]])
    div_flux = divergence(rhoV, X, bc='n', order=2)
    # Physical diffusion (viscosity)
    nu = 0.01  # Viscosity coefficient - adjust as needed
    lap_rho = laplacian(rho, X, bc='n', order=2)
    
    # Advection + Diffusion
    # return -grad_rho_dot_v - rho * div_v + nu * lap_rho
    return -div_flux + nu * lap_rho

# Setup logging
log_file = "simdata.txt"
if not os.path.exists(log_file):
    with open(log_file, 'w') as f:
        f.write("#" + 100 * "-"+"\n")
        f.write("# Simulation Data Log\n")
        f.write(f"# Started at: {time.ctime()}\n")
        f.write("# Step    Total_Mass   Mass_Change   Max_Density   Frame\n")
        f.write("#" + 100 * "-" + "\n")

# Main RK4 loop
for i, ti in enumerate(t):
    k1 = drho_dt(rho)
    k2 = drho_dt(rho + 0.5*dt*k1)
    k3 = drho_dt(rho + 0.5*dt*k2)
    k4 = drho_dt(rho + dt*k3)
    
    rho = rho + (dt/6.0) * (k1 + 2*k2 + 2*k3 + k4)

    if i % 5 == 0:
        current_mass = np.sum(rho)
        #print(f"Step {i:04d}: Mtot = {current_mass:.6f} ({current_mass - initial_mass:+.5f}) rho_max = {np.max(rho):>8.4f} frame = {frames:03d}/{int(total_time/(dt*5))+1}")
        with open(log_file, 'a') as f:
            f.write(
                f"Step {i:04d}: Mtot = {current_mass:.6f} ({current_mass - initial_mass:+.5f}) rho_max = {np.max(rho):>8.4f} frame = {frames:03d}/{total_frames}\n"
            )        
        # Store frame instead of showing
        fig, ax = plt.subplots(figsize=(4,4), dpi=300)
        im = ax.imshow(
            rho.T,
            cmap="inferno",
            origin="lower",
            extent=[x.min(), x.max(), y.min(), y.max()],
            vmin=0,
            vmax=1
        )
        ax.set_xlabel("$x$")
        ax.set_ylabel("$y$")
        ax.set_xticks(range(-4,5))
        ax.set_yticks(range(-4,5))
        ax.set_title(f"$t = {ti:.2f}$ s")
        plt.tight_layout()        
        plt.savefig(f"{temp_dir}/frame_{i:06d}.png")
        plt.close()

        frames += 1
        update_progress(frames, total_frames, time_start)

# Final mass check
final_mass = np.sum(rho)
print(f"\n\tInitial mass: {initial_mass:.6f}")
print(f"\tFinal mass: {final_mass:.6f}")
print(f"\tMass change: {final_mass - initial_mass:.6f}")
time_end = time.time()
print(f"Finished. Process completed in {format_time((time_end - time_start),ms=True)}")

import os

# At the end of your simulation script
print("Simulation complete. Creating animation...")
os.system("python animate.py")

# Clean up temp directory after animation is done
temp_dir = "temp"
if os.path.exists(temp_dir):
    print(f"Removing '{temp_dir}' directory...")
    shutil.rmtree(temp_dir)
    print(f"[/] Removed '{temp_dir}' directory")
print("Process complete")
print("="*100)

name = ahu
str1 = f"Wallahi {name}"
str2 = r"Macho\\all"
