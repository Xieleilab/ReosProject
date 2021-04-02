/***************************************************************************
  reosmeteorologicmodel.h - ReosMeteorologicModel

 ---------------------
 begin                : 16.2.2021
 copyright            : (C) 2021 by Vincent Cloarec
 email                : vcloarec at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef REOSMETEOROLOGICMODEL_H
#define REOSMETEOROLOGICMODEL_H

#include <QPointer>
#include <QIdentityProxyModel>
#include <QMimeData>

#include "reoswatershed.h"
#include "reosrainfallitem.h"

class ReosWatershedTree;
class ReosRainfallRegistery;
class ReosWatershedItemModel;

//! Class that handle association between watesheds and rainfalls
class REOSCORE_EXPORT ReosMeteorologicModel : public ReosDataObject
{
  public:
    ReosMeteorologicModel( const QString &name, QObject *parent = nullptr );
    ReosMeteorologicModel( const ReosEncodedElement &element,
                           ReosWatershedTree *watershedTree,
                           ReosRainfallRegistery *rainfallregistery,
                           QObject *parent = nullptr );

    QString type() const override {return QStringLiteral( "meteorologic-model" );}

    //! Returns a pointer to a copy of \a this
    ReosMeteorologicModel *duplicate( const QString &dupplicateName );

    //! Returns the name parameter
    ReosParameterString *name() const;
    //! Associates a \a rainfall with the \a watershed
    void associate( ReosWatershed *watershed, ReosRainfallSerieRainfallItem *rainfall );

    //! Disassociation the rainfall associated with the \a watershd
    void disassociate( ReosWatershed *watershed );

    //! Returns the associated rainfall of \a watershed
    ReosRainfallSerieRainfallItem *associatedRainfall( ReosWatershed *watershed ) const;

    //! Remove all reference with no association
    void purge() const;

    ReosEncodedElement encode( ReosWatershedTree *watershedTree ) const;

  private:
    std::unique_ptr<ReosParameterString> mName;
    using WatershedRainfallAssociation = QPair<QPointer<ReosWatershed>, QPointer<ReosRainfallSerieRainfallItem>>;
    mutable QList<WatershedRainfallAssociation> mAssociations;

    //! Searchs for \a watershed, if found , return its index, otherwise return -1
    int findWatershed( ReosWatershed *watershed ) const;
};

//! List model class that represents a collection of meteorologic model
class REOSCORE_EXPORT ReosMeteorologicModelsCollection : public QAbstractListModel
{
  public:
    ReosMeteorologicModelsCollection();

    int rowCount( const QModelIndex & ) const override;
    QModelIndex index( int row, int column, const QModelIndex & ) const override;
    QModelIndex parent( const QModelIndex & ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    //! Returns the meteorologic model at position \a i
    ReosMeteorologicModel *meteorologicModel( int i ) const;

    //! Returns the count of meteorologic models
    int modelCount() const;

    //! Adds an empty (no association) meteorologic model with \a name
    void addMeteorologicModel( const QString &name );

    //! Adds an existing meteorologic \a model, takes ownership
    void addMeteorologicModel( ReosMeteorologicModel *model );

    //! Removes the meteorologic model at position \a i
    void removeMeteorologicModel( int i );

    void clearModels();

    ReosEncodedElement encode( ReosWatershedTree *watershedTree ) const;
    void decode( const ReosEncodedElement &element, ReosWatershedTree *watershedTree, ReosRainfallRegistery *rainfallregistery );

  private:
    QVector<ReosMeteorologicModel *> mMeteoModels;
};


//! Item model class that represents association between watershed and rainfall for a given meteorologic model
class REOSCORE_EXPORT ReosMeteorologicItemModel: public QIdentityProxyModel
{
  public:

    ReosMeteorologicItemModel( ReosWatershedItemModel *watershedModel, QObject *parent = nullptr );

    QVariant data( const QModelIndex &index, int role )  const override;
    int columnCount( const QModelIndex & ) const override;
    bool canDropMimeData( const QMimeData *data, Qt::DropAction, int, int, const QModelIndex &parent ) const override;
    bool dropMimeData( const QMimeData *data, Qt::DropAction, int, int, const QModelIndex &parent ) override;
    Qt::ItemFlags flags( const QModelIndex &index ) const override;
    Qt::DropActions supportedDropActions() const override;
    QStringList mimeTypes() const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;

    //! Sets the current meteorologic model
    void setCurrentMeteorologicalModel( ReosMeteorologicModel *meteoModel );
    //! Removes the association for watershed at \a index
    void removeAssociation( const QModelIndex &index );

  private:
    ReosWatershedItemModel *mWatershedModel = nullptr;
    size_t currentConfig = 0;

    ReosMeteorologicModel *mCurrentMeteoModel = nullptr;

    ReosRainfallSerieRainfallItem *rainfallInMeteorologicModel( const QModelIndex &index );
    ReosRainfallSerieRainfallItem *rainfallInRainfallModel( const QString &uri ) const;
};

#endif // REOSMETEOROLOGICMODEL_H
