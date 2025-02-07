/***************************************************************************
  reosmesh.h - ReosMesh

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
#ifndef REOSMESH_H
#define REOSMESH_H

#include <QPointF>
#include <QVector>
#include <QPointer>
#include <QSet>

#include "reoscore.h"
#include "reosrenderedobject.h"
#include "reosencodedelement.h"

class ReosMeshGenerator;
class ReosMeshFrameData;
class ReosDigitalElevationModel;
class ReosMap;
class ReosTopographyCollection;
class ReosParameterDouble;
class ReosParameterInteger;
class ReosParameterSlope;
class ReosParameterArea;
class ReosHydraulicSimulationResults;
class ReosSpatialPosition;
class ReosMeshDatasetSource;


class ReosMeshQualityChecker : public ReosProcess
{
  public:
    virtual ~ReosMeshQualityChecker() {}
    struct QualityMeshResults
    {
      QString error;
      int errorVertex = -1;
      int errorFace = -1;
      QList<QPolygonF> minimumAngle;
      QList<QPolygonF> maximumAngle;;
      QList<QPointF> connectionCount;
      QList<QPointF> connectionCountBoundary;
      QList<QLineF> maximumSlope;
      QList<QPolygonF> minimumArea;
      QList<QPolygonF> maximumArea;
      QList<QPolygonF> maximumAreaChange;
    };

    virtual QualityMeshResults result() const = 0;

  protected:
    mutable QualityMeshResults mResult;
};


class REOSCORE_EXPORT ReosMesh: public ReosRenderedObject
{
    Q_OBJECT
  public:

    struct QualityMeshParameters
    {
      ReosParameterDouble *minimumAngle = nullptr;
      ReosParameterDouble *maximumAngle = nullptr;
      ReosParameterInteger *connectionCount = nullptr;
      ReosParameterInteger *connectionCountBoundary = nullptr;
      ReosParameterSlope *maximumSlope = nullptr;
      ReosParameterArea *minimumArea = nullptr;
      ReosParameterArea *maximumArea = nullptr;
      ReosParameterDouble *maximumAreaChange = nullptr;

      ReosEncodedElement encode() const;
      void decode( const ReosEncodedElement &element, QObject *parent );
    };

    enum QualityMeshCheck
    {
      MinimumAngle = 1 << 0,
      MaximumAngle = 1 << 1,
      ConnectionCount = 1 << 2,
      ConnectionCountBoundary = 1 << 3,
      MaximumSlope = 1 << 4,
      MinimumArea = 1 << 5,
      MaximumArea = 1 << 6,
      MaximumAreaChange = 1 << 7
    };
    Q_ENUM( QualityMeshCheck )
    Q_DECLARE_FLAGS( QualityMeshChecks, QualityMeshCheck )
    Q_FLAG( QualityMeshChecks )

    struct WireFrameSettings
    {
      bool enabled = false;
      QColor color;
      double width = 0.2;
    };

    //! Creates a new void mesh in memory
    static ReosMesh *createMeshFrame( const QString &crs = QString(), QObject *parent = nullptr );

    static ReosMesh *createMeshFrameFromFile( const QString &dataPath );

    //! Returns whether the mesh is valid
    virtual bool isValid() const = 0;

    //! Returns vertex count
    virtual int vertexCount() const = 0;

    //! Returns face count
    virtual int faceCount() const = 0;

    //! Returns whether the mesh frame is modified
    virtual bool isFrameModified() const = 0;

    //! Clears ans generates a new mesh with \a data
    virtual void generateMesh( const ReosMeshFrameData &data ) = 0;

    virtual QString crs() const = 0;

    virtual QVector<int> face( int faceIndex ) const = 0 ;

    virtual QPointF vertexPosition( int vertexIndex, const QString &destinationCrs = QString() ) = 0;

    virtual QObject *data() const = 0;

    /**
     * Activate vertices elevation as a dataset group with \a name, returns a unique id of this dataset group.
     */
    virtual QString enableVertexElevationDataset( const QString &name ) = 0;

    //! Returns the vertices elevation dataset id
    virtual QString verticesElevationDatasetId() const = 0;

    //! Returns all the dataset ids contained in the mesh
    virtual QStringList datasetIds() const = 0;

    //! Returns the name of the dataset with \a id
    virtual QString datasetName( const QString &id ) const = 0;

    //! Activates the dataset with \a id
    virtual bool activateDataset( const QString &id, bool update = true ) = 0;

    //! Returns the current scalar dataset Id
    virtual QString currentdScalarDatasetId() const = 0;

    //! Returns an index correspondng to the dataset group with \a id
    virtual int datasetGroupIndex( const QString &id ) const = 0;

    //! Returns whether the mesh has a dataset group with \a id
    virtual bool hasDatasetGroupIndex( const QString &id ) const = 0;

    //! Returns a process that apply the topography collection on the mesh, caller take ownership
    virtual ReosProcess *applyTopographyOnVertices( ReosTopographyCollection *topographyCollection ) = 0;

    //! Returns the value of dataset \a datasetId at position \a pos in map coordinates
    virtual double datasetScalarValueAt( const QString &datasetId, const QPointF &pos ) const = 0;

    //! Save the mesh frame on UGRID file with path \a dataPath
    virtual void save( const QString &dataPath ) = 0;

    virtual void stopFrameEditing( bool commit, bool continueEditing = false ) = 0;

    virtual ReosEncodedElement meshSymbology() const = 0;

    virtual void setMeshSymbology( const ReosEncodedElement &symbology ) = 0;

    virtual ReosEncodedElement datasetScalarGroupSymbology( const QString &id ) const = 0;

    virtual void setDatasetScalarGroupSymbology( const ReosEncodedElement &encodedElement, const QString &id ) = 0;

    virtual void activateWireFrame( bool activate ) = 0;
    virtual bool isWireFrameActive() const = 0;

    virtual WireFrameSettings wireFrameSettings() const = 0 ;

    virtual void setWireFrameSettings( const WireFrameSettings &wireFrameSettings ) = 0;

    virtual void update3DRenderer() = 0;

    //! Returns a process that check the quality of the mesh, caller take ownership
    virtual ReosMeshQualityChecker *getQualityChecker( QualityMeshChecks qualitiChecks, const QString &destinatonCrs ) const = 0;

    virtual void setSimulationResults( ReosHydraulicSimulationResults *result ) = 0;

    virtual double interpolateDatasetValueOnPoint( const ReosMeshDatasetSource *datasetSource, const ReosSpatialPosition &position, int sourceGroupindex, int datasetIndex ) const = 0;

    QualityMeshParameters qualityMeshParameters() const;
    void setQualityMeshParameter( const ReosEncodedElement &element );

    void setBoundariesVertices( const QVector<QVector<int> > &vertices );
    void setHolesVertices( const QVector<QVector<QVector<int> > > &vertices );

    bool vertexIsOnBoundary( int vertexIndex ) const;
    bool vertexIsOnHoleBorder( int vertexIndex ) const;

    virtual QString verticalDataset3DId() const = 0;
    virtual void setVerticalDataset3DId( const QString &verticalDataset3DId, bool update = true ) = 0;

    double verticaleSCale() const;
    void setVerticaleSCale( double verticaleSCale );

  protected:
    ReosMesh( QObject *parent = nullptr );

    QualityMeshParameters mQualityMeshParameters;
    QSet<int> mBoundaryVerticesSet;
    QSet<int> mHolesVerticesSet;


    double mVerticaleSCale = 1;
};

#endif // REOSMESH_H
