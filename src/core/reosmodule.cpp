/***************************************************************************
                      reosmodule.cpp
                     --------------------------------------
Date                 : 18-11-2018
Copyright            : (C) 2018 by Vincent Cloarec
email                : vcloarec at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtConcurrent>
#include <QFuture>

#include "reosmodule.h"
#include "reosprocess.h"

ReosModule::ReosModule( QObject *parent ):
  QObject( parent ),  mGroupAction( new QActionGroup( this ) )
{
  mReosParent = qobject_cast<ReosModule *>( parent );

  if ( mReosParent )
    connect( this, &ReosModule::dirtied, mReosParent, &ReosModule::dirtied );
}

ReosModule::~ReosModule()
{
  if ( mReosParent )
    mReosParent->mReosChildren.removeOne( this );
}

void ReosModule::newCommand( QUndoCommand *command )
{
  if ( mUndoStack )
  {
    mUndoStack->push( command );
    return;
  }

  if ( mReosParent )
  {
    mReosParent->newCommand( command );
  }
  else
  {
    emit newCommandToUndoStack( command );
  }
}

void ReosModule::warning( QString message, bool inMessageBox ) const
{
  sendMessage( message, Warning, inMessageBox );
}

void ReosModule::error( QString message, bool inMessageBox ) const
{
  sendMessage( message, Error, inMessageBox );
}

void ReosModule::message( QString message, bool inMessageBox ) const
{
  sendMessage( message, Simple, inMessageBox );
}

void ReosModule::order( QString message, bool inMessageBox ) const
{
  sendMessage( message, Order, inMessageBox );
}

void ReosModule::message( const ReosModule::Message &messageObject, bool inMessageBox )
{
  switch ( messageObject.type )
  {
    case ReosModule::Simple:
      message( messageObject.text, inMessageBox );
      break;
    case ReosModule::Order:
      order( messageObject.text, inMessageBox );
      break;
    case ReosModule::Warning:
      warning( messageObject.text, inMessageBox );
      break;
    case ReosModule::Error:
      error( messageObject.text, inMessageBox );
      break;
  }
}

void ReosModule::onMessageReceived( const QString &message, const ReosModule::MessageType &type, bool inMessageBox )
{
  sendMessage( message, type, inMessageBox );
}

void ReosModule::sendMessage( QString mes, MessageType type, bool messageBox ) const
{
  if ( mReosParent )
    mReosParent->sendMessage( mes, type, messageBox );
  else
    emit emitMessage( {type, mes}, messageBox );
}

QList<QAction *> ReosModule::actions() const {return mGroupAction->actions();}

void ReosModule::redo()
{
  if ( mUndoStack )
    mUndoStack->redo();
}

void ReosModule::undo()
{
  if ( mUndoStack )
    mUndoStack->undo();
}


void ReosModule::Message::prefixMessage( const QString &prefix )
{
  text.prepend( prefix );
}

void ReosModule::Message::addText( const QString &newText )
{
  if ( !text.isEmpty() )
  {
    text.append( "\n" );
    text.append( newText );
  }
  else
    text = newText;
}
