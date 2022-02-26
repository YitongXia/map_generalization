# Map Generalization

the idea comes from: https://3d.bk.tudelft.nl/courses/geo1004//hw/1/

this program will implement the generalised map on the surface:

1. read a 3D polygonal mesh from an OBJ file,
2. store the polygonal mesh in a generalised map,
3. output all the darts and cells from the generalised map to .csv files, and finally
4. output a triangulation of the polygonal mesh to a new OBJ file.

The object before generalization:

![1645905685(1)](https://user-images.githubusercontent.com/75926656/155857475-beec3c36-ec4c-4b50-ad16-d2c507c43317.png)

The object after generalization

![1645905634](https://user-images.githubusercontent.com/75926656/155857447-f5506652-5a62-48bb-a97b-02c7f99815c7.png)

the input data(obj file) contains:
1. a single valid polygonal mesh (so only one volume)
2. whose faces are all convex and contain no holes (Face normals are implicit from the vertex order. The orientation of the vertices of the faces in relation to the face normal follows the right-hand rule)


the output files:
1. one CSV file for the darts (filename ends on darts.csv) with the columns ID, a0, a1, a2, a3, v, e, and f.
2. one CSV file for the 0-cells (ending on vertices.csv) with  the columns ID, dart, x, y, and z.
3. one CSV file for the 1-cells (ending on edges.csv) with the columns ID, dart.
4. one CSV file for the 2-cells (ending on faces.csv) with the columns ID, dart.
5. one CSV file for the 3-cell (ending on volume.csv) with the columns ID, dart.
