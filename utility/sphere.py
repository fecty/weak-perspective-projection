import numpy as np
from math import sin, cos, pi


def generate_sphere_points(size=1, N=20, digits=4):
    """Generate points on a sphere surface."""
    s = size

    theta_arr = np.linspace(0, 2 * pi, N)
    phi_arr = np.linspace(-pi / 2, pi / 2, N)

    points = []

    for phi in phi_arr:
        for theta in theta_arr:
            x = round(s * cos(phi) * cos(theta), digits)
            y = round(s * cos(phi) * sin(theta), digits)
            z = round(s * sin(phi), digits)
            points.append((x, y, z))

    return points, len(theta_arr), len(phi_arr)


def get_cpp_nodes(points):
    """Print node coordinates."""

    nodeStr = ""
    for x, y, z in points:
        # print(f"{{{x}f,{y}f,{z}f}},", end="")
        nodeStr += f"{{{x}f,{y}f,{z}f}},"
    # print("\n" + "-" * 50 + "\n")

    # print(nodeStr)
    # print(f"""vector<Vector3> nodes = {{{nodeStr}}};\n""")
    # print(nodeStr + "\n\n")
    return f"""vector<Vector3> nodes = {{{nodeStr}}};\n"""


def get_cpp_edges(theta_count, phi_count):
    """Print edges based on grid connectivity."""
    total = theta_count * phi_count
    edgeStr = ""

    for i in range(total):
        # horizontal neighbor
        if (i + 1) % theta_count != 0:
            # print(f"{{{i},{i + 1}}},", end="")
            edgeStr += f"{{{i},{i + 1}}},"

        # vertical neighbor
        j = i + theta_count
        if j < total:
            # print(f"{{{i},{j}}},", end="")
            edgeStr += f"{{{i},{j}}},"

    # print(f"""vector<vector<int>> edges = {{{edgeStr}}};""")
    return f"""vector<vector<int>> edges = {{{edgeStr}}};"""


def sphere(size=1, N=20, digits=4):
    points, theta_count, phi_count = generate_sphere_points(size, N, digits)

    # print(get_cpp_nodes(points))
    # print(get_cpp_edges(theta_count, phi_count))

    fileName = "src/shape.cpp"
    try:
        with open("src/shape.cpp", "w+") as f:
            f.write(
                f"""#include <vector>
        #include <raylib.h>
        #include "shape.hpp"
        using namespace std;
        {get_cpp_nodes(points)}
        {get_cpp_edges(theta_count, phi_count)}"""
            )
        print(f"written to {fileName}")
    except Exception as e:
        print(e)

    return points


def main():
    sphere(N=40, size=2)


if __name__ == "__main__":
    main()
