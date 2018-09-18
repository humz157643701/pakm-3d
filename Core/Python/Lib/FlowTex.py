from TexGen.Core import *

def ExportFlowTex(Filename, TextileName, NumPoints):
    """ Export a textile to be read in by FlowTex. """
    # Open file for writing
    File = open(Filename, 'w')

    # Get the textile
    Textile = GetTextile(TextileName)

    # Get the limits of the domain
    Min, Max = Textile.GetDomain().GetMesh().GetAABB()

    # Calculate spacing based on domain size and number of points
    Spacing = [0,0,0]
    Spacing[0] = (Max.x-Min.x)/(NumPoints[0])
    Spacing[1] = (Max.y-Min.y)/(NumPoints[1])
    Spacing[2] = (Max.z-Min.z)/(NumPoints[2])

    # Write spacing and number of points out to file 
    File.write(TextileName + '\n')
    for Item in Spacing:
        File.write(str(Item) + '\n')
    for Item in NumPoints:
        File.write(str(Item) + '\n')

    # Create a 3D grid of points that will be used to look up information
    # from TexGen
    Points = []
    for i in range(NumPoints[0]):
        for j in range(NumPoints[1]):
            for k in range(NumPoints[2]):
                Point = XYZ(i*Spacing[0]+Min.x, j*Spacing[1]+Min.y, k*Spacing[2]+Min.z)
                Points.append(Point)

    # Create a list to store information returned from TexGen
    PointsInfo = PointInfoVector()

    # Get point information for the points created
    Textile.GetPointInformation(Points, PointsInfo)

    # Output the information to the file 
    for PointInfo in PointsInfo:
        # If the yarn index is -1 that means its a matrix point
        if PointInfo.iYarnIndex == -1:
            File.write('F')
        else:
            File.write('S')

    # Don't bother calculated volume fraction and porosity because it is not
    # used, just output 0's
    File.write('\n0\n0\n0\n0\n')

