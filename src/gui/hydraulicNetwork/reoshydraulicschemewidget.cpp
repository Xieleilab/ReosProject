/***************************************************************************
  reoshydraulicschemewidget.cpp - ReosHydraulicSchemeWidget

 ---------------------
 begin                : 25.3.2022
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
#include "reoshydraulicschemewidget.h"
#include "ui_reoshydraulicschemewidget.h"

#include "reoshydraulicscheme.h"
#include "reoshydraulicnetwork.h"
#include "reoswatershedmodule.h"
#include "reosmeteorologicmodel.h"

ReosHydraulicSchemeWidget::ReosHydraulicSchemeWidget( ReosHydraulicScheme *scheme, const ReosHydraulicNetworkContext &context, QWidget *parent )
  : QWidget( parent )
  , ui( new Ui::ReosHydraulicSchemeWidget )
  , mContext( context )
{
  ui->setupUi( this );
  ui->mMeteoModelCombo->setModel( context.watershedModule()->meteoModelsCollection() );
  setScheme( scheme );

  connect( ui->mMeteoModelCombo, QOverload<int>::of( &QComboBox::currentIndexChanged ), this, &ReosHydraulicSchemeWidget::onMeteoModelChange );
}

ReosHydraulicSchemeWidget::~ReosHydraulicSchemeWidget()
{
  delete ui;
}

void ReosHydraulicSchemeWidget::setScheme( ReosHydraulicScheme *scheme )
{
  mScheme = scheme;
  if ( scheme )
  {
    ui->mWidetName->setString( scheme->schemeName() );
    ui->mWidgetStartTime->setDateTime( scheme->startTime() );
    ui->mWidgetEndTime->setDateTime( scheme->endTime() );
    int meteoModelindex = mContext.watershedModule()->meteoModelsCollection()->modelIndex( scheme->meteoModel() );

    if ( meteoModelindex != -1 )
      ui->mMeteoModelCombo->setCurrentIndex( meteoModelindex );
  }
}

void ReosHydraulicSchemeWidget::onMeteoModelChange()
{
  mScheme->setMeteoModel( mContext.watershedModule()->meteoModelsCollection()->meteorologicModel( ui->mMeteoModelCombo->currentIndex() ) );
}

ReosHydraulicSchemeWidgetAction::ReosHydraulicSchemeWidgetAction( ReosHydraulicNetwork *network, QObject *parent )
  : QWidgetAction( parent )
  , mNetwork( network )
{}

void ReosHydraulicSchemeWidgetAction::setCurrentScheme( ReosHydraulicScheme *scheme )
{
  mScheme = scheme;
  mWidget->setScheme( scheme );
}

QWidget *ReosHydraulicSchemeWidgetAction::createWidget( QWidget *parent )
{
  mWidget = new ReosHydraulicSchemeWidget( mScheme, mNetwork->context(), parent );
  return mWidget;
}
