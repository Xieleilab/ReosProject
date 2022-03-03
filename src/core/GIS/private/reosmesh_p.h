/***************************************************************************
  reosmesh_p.h - ReosMesh_p

 ---------------------
 begin                : 13.1.2022
 copyright            : (C) 2022 by Vincent Cloarec
 email                : vcloarec at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef REOSMESH_P_H
#define REOSMESH_P_H

#include <qgsmeshlayer.h>

#include "reosmesh.h"

class ReosMeshDataProvider_p;
class ReosMeshFrameData;
class QgsMeshDatasetGroup;
class ReosDigitalElevationModel;

/**
 * Implementation of a mesh in Reos environment.
 * This class contains a QgsMeshLayer that can be independant from the QgsProject.
 * The data provider of this QGIS layer is a derived class of QgsMeshDataProvider that allow creation of mesh in memory
 * and custom behaviors, especially mesh generation and editing.
 */
class ReosMesh_p : public ReosMesh
{
  public:
    ReosMesh_p( const QString &crs, QObject *parent = nullptr );
    ReosMesh_p( const ReosEncodedElement &elem, const QString &dataPath );
    bool isValid() const override;
    void addVertex( const QPointF pt, double z, double tolerance ) override;
    int vertexCount() const override;
    int faceCount() const override;
    void render( QGraphicsView *canvas, QPainter *painter ) override;
    QString enableVertexElevationDataset( const QString &name ) override;
    bool activateDataset( const QString &id ) override;
    void generateMesh( const ReosMeshFrameData &data ) override;
    QString crs() const override;
    QObject *data() const override;
    int datasetGroupIndex( const QString &id ) const override;
    void applyTopographyOnVertices( ReosTopographyCollection *topographyCollection ) override;

    ReosEncodedElement encode( const QString &dataPath ) const override;

  private:

    std::unique_ptr<QgsMeshLayer> mMeshLayer;
    ReosMeshDataProvider_p *meshProvider() const;
    QMap<QString, int> mDatasetGroupsIndex;
    QgsMeshDatasetGroup *mZVerticesDatasetGroup = nullptr;
    int mVerticesElevationDatasetIndex = -1;


    void init();
    void activateVertexZValueDatasetGroup();
    QString addDatasetGroup( QgsMeshDatasetGroup *group );
    void firstUpdateOfTerrainScalarSetting();
};

#endif // REOSMESH_P_H
