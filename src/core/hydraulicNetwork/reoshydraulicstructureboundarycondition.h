/***************************************************************************
  reoshydraulicstructureboundarycondition.h - ReosHydraulicStructureBoundaryCondition

 ---------------------
 begin                : 23.3.2022
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
#ifndef REOSHYDRAULICSTRUCTUREBOUNDARYCONDITION_H
#define REOSHYDRAULICSTRUCTUREBOUNDARYCONDITION_H

#include "reoscore.h"
#include "reoshydrographsource.h"
#include "reoshydraulicnetwork.h"

class ReosHydraulicStructure2D;
class ReosTimeSeriesVariableTimeStepGroup;

class REOSCORE_EXPORT ReosHydraulicStructureBoundaryCondition : public ReosHydrographJunction
{
    Q_OBJECT
  public:
    enum class Type
    {
      NotDefined,
      InputFlow,
      OutputLevel,
    };

    enum class ConnectionState
    {
      NotConnected,
      ConnectedToUpstreamLink,
      ConnectedToDownstreamLink,
    };

    ReosHydraulicStructureBoundaryCondition(
      ReosHydraulicStructure2D *hydStructure,
      const QString &boundaryConditionId,
      const ReosHydraulicNetworkContext &context );

    QString type() const override {return staticType();}
    static QString staticType() {return ReosHydrographJunction::staticType() + QString( ':' ) + QStringLiteral( "structure-boundary-condition" );}
    static ReosHydraulicStructureBoundaryCondition *decode( const ReosEncodedElement &encodedElement, const ReosHydraulicNetworkContext &context );

    QPointF position( const QString &destinationCrs ) const override;
    void setPosition( const ReosSpatialPosition & ) override {};
    bool isAutoSelectable() const override;
    bool isRemovable() const override {return false;}
    bool canAcceptLink( const QString &linkId, int positionInLink ) override;
    void updateCalculationContextFromUpstream( const ReosCalculationContext &context, ReosHydrographRoutingLink *upstreamLink, bool upstreamWillChange ) override;
    virtual void saveConfiguration( ReosHydraulicScheme *scheme ) const override;
    void restoreConfiguration( ReosHydraulicScheme *scheme ) override;
    QString outputPrefixName() const override;

    QString boundaryConditionId() const;

    void attachStructure( ReosHydraulicStructure2D *structure );

    Type conditionType() const;

    ConnectionState connectionState() const;

    ReosHydraulicStructureBoundaryCondition::Type defaultConditionType() const;
    void setDefaultConditionType( const ReosHydraulicStructureBoundaryCondition::Type &defaultConditionType );

    ReosParameterBoolean *isWaterLevelConstant() const;

    ReosParameterDouble *constantWaterElevation() const;

    ReosTimeSeriesVariableTimeStepGroup *waterLevelSeriesGroup() const;
    int waterLevelSeriesIndex() const;
    void setWaterLevelSeriesIndex( int waterLevelSeriesIndex );
    ReosTimeSerieVariableTimeStep *waterLevelSeries() const;

  public slots:
    //void updateCalculationContext( const ReosCalculationContext &context );
    //virtual void onUpstreamRoutingUpdated( const QString &routingId ) {}

  protected:
    ReosHydraulicStructureBoundaryCondition( const ReosEncodedElement &encodedElement, ReosHydraulicNetwork *parent = nullptr );
    void encodeData( ReosEncodedElement &, const ReosHydraulicNetworkContext & ) const override;

  private slots:
    void onBoundaryClassesChange();
    void onParameterNameChange();

  private:
    ReosHydraulicNetworkContext mContext;
    QPointer<ReosHydraulicStructure2D> mStructure;
    QString mBoundaryConditionId;
    ReosTimeSeriesVariableTimeStepGroup *mWaterLevelSeriesGroup = nullptr;

    //** config attribute
    ReosHydraulicStructureBoundaryCondition::Type mDefaultConditionType = ReosHydraulicStructureBoundaryCondition::Type::OutputLevel;
    ReosParameterBoolean *mIsWaterLevelConstant = nullptr;
    ReosParameterDouble *mConstantWaterLevel = nullptr;
    int mWaterLevelSeriesIndex = -1;
    //**

    void init();
    void syncHydrographName();
    void loadHydrographResult( const ReosCalculationContext &calculationContext );
};

class ReosHydraulicStructureBoundaryConditionFactory : public ReosHydraulicNetworkElementFactory
{
  public:
    ReosHydraulicStructureBoundaryConditionFactory() = default;
    ReosHydraulicNetworkElement *decodeElement( const ReosEncodedElement &encodedElement, const ReosHydraulicNetworkContext &context ) const override;
};

#endif // REOSHYDRAULICSTRUCTUREBOUNDARYCONDITION_H
