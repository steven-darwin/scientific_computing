import subprocess as sp
import os
from pathlib import Path

for p in Path("../build/staging/").glob("slab*"):
    p.unlink()

sp.run(['../build/geometry_mesher/GeometryMesher.exe'], cwd = '../build/geometry_mesher/')
sp.run(['../build/multi_physics_solver/MultiPhysicsSolver.exe'], cwd = '../build/multi_physics_solver/')
sp.run(['python', 'point_iterative_visualization.py'], cwd = '../module/simulation_visualizer/script/')