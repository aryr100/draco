/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com>
# All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
#
# This file incorporates work covered by the following copyright and
# permission notice:
#
# Lumina Desktop Environment (https://lumina-desktop.org)
# Copyright (c) 2012-2017, Ken Moore (moorekou@gmail.com)
# All rights reserved
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the organization nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
*/

//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012-2015, Ken Moore
//  Available under the 3-clause BSD license
//===========================================
#include "LClock.h"
#include "LSession.h"
//#include <LuminaThemes.h>
#include <LuminaXDG.h>

LClock::LClock(QWidget *parent, QString id, bool horizontal) : LPPlugin(parent, id, horizontal){
  button = new QToolButton(this); //RotateToolButton(this);
    button->setAutoRaise(true);
    button->setToolButtonStyle(Qt::ToolButtonTextOnly);
    //button->setStyleSheet("font-weight: bold;");
    button->setPopupMode(QToolButton::DelayedPopup); //make sure it runs the update routine first
    button->setMenu(new QMenu());
//    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  //  this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    connect(button, SIGNAL(clicked()), this, SLOT(openMenu()));
    connect(button->menu(), SIGNAL(aboutToHide()), this, SIGNAL(MenuClosed()));
  calendar = new QCalendarWidget(this);
  calAct = new QWidgetAction(this);
	calAct->setDefaultWidget(calendar);
//  TZMenu = new QMenu(this);
    //connect(TZMenu, SIGNAL(triggered(QAction*)), this, SLOT(ChangeTZ(QAction*)) );

  //Now assemble the menu
  button->menu()->addAction(calAct);
  //button->menu()->addMenu(TZMenu);

  //button->setStyleSheet("background-color:red;");

  //button->setContentsMargins(10,0,10,0);
  //this->layout()->setContentsMargins(0,0,0,0); //reserve some space on left/right
  this->layout()->addWidget(button);

  //Setup the timer
  timer = new QTimer();
  //Load all the initial settings
  updateFormats();
  LocaleChange();
  ThemeChange();
  OrientationChange();
  //Now connect/start the timer
  connect(timer,SIGNAL(timeout()), this, SLOT(updateTime()) );
  connect(QApplication::instance(), SIGNAL(SessionConfigChanged()), this, SLOT(updateFormats()) );
  timer->start();
}

LClock::~LClock(){
  timer->stop();
  delete timer;
}


void LClock::updateTime(bool adjustformat){
  QDateTime CT = QDateTime::currentDateTime();
  //Now update the display
  QString label;
  QString timelabel;
  QString datelabel;
  if(deftime){ timelabel = CT.time().toString(Qt::DefaultLocaleShortDate) ; }
  else{ timelabel=CT.toString(timefmt); }
  if(defdate){ datelabel = CT.date().toString(Qt::DefaultLocaleShortDate); }
  else{ datelabel = CT.toString(datefmt); }
  if(datetimeorder == "dateonly"){
	  label = datelabel;
	  button->setToolTip(timelabel);
  }else if(datetimeorder == "timedate"){
	  label = timelabel + "\n" + datelabel;
	  button->setToolTip("");
  }else if(datetimeorder == "datetime"){
	  label = datelabel + "\n" + timelabel;
	  button->setToolTip("");
  }else{
	 label = timelabel;
	button->setToolTip(datelabel);
  }
  QStringList lines = label.split("\n");
  if( this->layout()->direction() == QBoxLayout::TopToBottom ){
    //different routine for vertical text (need newlines instead of spaces)
    for(int i=0; i<label.count("\n")+1; i++){
      if(this->size().width() < (this->fontMetrics().width(label.section("\n",i,i))+10 )&& label.section("\n",i,i).contains(" ")){
	label.replace(label.section("\n",i,i), label.section("\n",i,i).replace(" ", "\n"));
        i--;
      }
    }
    //label.replace(" ","\n");
  }else if( this->size().height() < lines.length()*this->fontMetrics().height() ){
    label.replace("\n",", ");
  }
  if(adjustformat){
      qDebug() << "adjust clock format";
    QFont font = LSession::handle()->font();
      font.setBold(true);


    bool foundOkSize = false;
    int fontSize = font.pointSize();
    bool isHoriz = this->layout()->direction()==QBoxLayout::LeftToRight;
    while (!foundOkSize) {
        qDebug() << "testing clock font size";
        QFontMetrics metrics(font);
        int metricValue = metrics.height();
        int panelValue = height()-5/* padding */;
        int dateLines = label.contains("\n")?label.count("\n"):0;
        if (dateLines==1) { dateLines++; }
        if (!isHoriz) {
            panelValue = width();
            metricValue = metrics.width(label)+10/* padding */;
        }

        if (dateLines>0) { panelValue = panelValue/dateLines; }
        if (metricValue<panelValue) { font.setPointSize(fontSize); }
        else { foundOkSize = true;break; }
        fontSize++;
    }


    button->setFont(font);
   //Check the font/spacing for the display and adjust as necessary
   /* QStringList lines = label.split("\n");
    QFontMetrics metrics(font);
    if(this->layout()->direction()==QBoxLayout::LeftToRight){
      //horizontal layout
      int wid = 0;
      int lwid = 0;
      for(int i=0; i<lines.length(); i++){
        lwid = metrics.width(lines[i]);
        if(lwid>wid){ wid = lwid; }
      }
     qDebug() << "Verify Clock width:" << lines.length() << wid << lines;
     this->setMinimumWidth(wid);
     this->setMaximumWidth(wid + (4*metrics.width("O")));
    }else{
      //vertical layout
      this->setMinimumHeight(metrics.lineSpacing() * lines.length());
      this->setMaximumHeight( (lines.length()+4)*metrics.height() );
    }*/
  }
  button->setText(label);
}

void LClock::updateFormats(){
  qDebug() << "Updating clock format";
  timefmt = LSession::handle()->sessionSettings()->value("TimeFormat","").toString();
  datefmt = LSession::handle()->sessionSettings()->value("DateFormat","").toString();
  deftime = timefmt.simplified().isEmpty();
  defdate = datefmt.simplified().isEmpty();
  //Adjust the timer interval based on the smallest unit displayed
  if(deftime){ timer->setInterval(500); } //1/2 second
  else if(timefmt.contains("z")){ timer->setInterval(1); } //every millisecond (smallest unit)
  else if(timefmt.contains("s")){ timer->setInterval(500); } //1/2 second
  else if(timefmt.contains("m")){ timer->setInterval(2000); } //2 seconds
  else{ timer->setInterval(1000); } //unknown format - use 1 second interval
  datetimeorder = LSession::handle()->sessionSettings()->value("DateTimeOrder", "timeonly").toString().toLower();
  //this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  updateTime(true);
  //Now fix the size of the widget with the new size hint
  //this->setFixedWidth( this->sizeHint().width() +6);
}

void LClock::updateMenu(){
  QDateTime cdt = QDateTime::currentDateTime();
  //TZMenu->setTitle(QString(tr("Time Zone (%1)")).arg(cdt.timeZoneAbbreviation()) );
  calendar->showToday(); //make sure the current month is visible
  calendar->setSelectedDate(QDate::currentDate()); //select the actual date for today
}

void LClock::openMenu(){
  updateMenu();
  button->showMenu();
}

void LClock::closeMenu(){
  button->menu()->hide();
}
	
/*void LClock::ChangeTZ(QAction *act){
  //LTHEME::setCustomEnvSetting("TZ",act->whatsThis());
  //QTimer::singleShot(500, this, SLOT(updateTime()) );
}*/

/*void LClock::LocaleChange(){
  //Refresh all the time zone information
  TZMenu->clear();
    TZMenu->addAction(tr("Use System Time"));
    TZMenu->addSeparator();
  QList<QByteArray> TZList = QTimeZone::availableTimeZoneIds();
  //Orgnize time zones for smaller menus (Continent/Country/City)
  // Note: id = Continent/City
  QStringList info;
  for(int i=0; i<TZList.length(); i++){
    QTimeZone tz(TZList[i]);
    if(!QString(tz.id()).contains("/")){ continue; }
    info << "::::"+QString(tz.id()).section("/",0,0)+"::::"+QLocale::countryToString(tz.country())+"::::"+QString(tz.id()).section("/",1,100).replace("_"," ")+"::::"+QString(tz.id());
  }
  //Now sort alphabetically
  info.sort();
  //Now create the menu tree
  QString continent, country; //current continent/country
  QMenu *tmpC=0; //continent menu
  QMenu *tmpCM=0; //country menu
  for(int i=0; i<info.length(); i++){
    //Check if different continent
    if(info[i].section("::::",1,1)!=continent){
      if(tmpC!=0){
        if(tmpCM!=0 && !tmpCM->isEmpty()){
	  tmpC->addMenu(tmpCM);
	}
	if(!tmpC->isEmpty()){ TZMenu->addMenu(tmpC); }
      }
      tmpC = new QMenu(this);
	tmpC->setTitle(info[i].section("::::",1,1));
      tmpCM = new QMenu(this);
	  tmpCM->setTitle(info[i].section("::::",2,2));
    //Check if different country
    }else if(info[i].section("::::",2,2)!=country){
        if(tmpC!=0 && tmpCM!=0 && !tmpCM->isEmpty()){
	  tmpC->addMenu(tmpCM);
	}
	tmpCM = new QMenu(this);
	  tmpCM->setTitle(info[i].section("::::",2,2));
    }
    //Now create the entry within the country menu
    if(tmpCM!=0){
      QAction *act = new QAction(info[i].section("::::",3,3), this);
	act->setWhatsThis(info[i].section("::::",4,4) );
      tmpCM->addAction(act);
    }
    //Save the values for the next run
    continent = info[i].section("::::",1,1);
    country = info[i].section("::::",2,2);

    if(i== info.length()-1){
      //last go through - save all menus
      if(tmpCM!=0 && tmpC!=0 && !tmpCM->isEmpty()){ tmpC->addMenu(tmpCM); }
      if(tmpC!=0 && !tmpC->isEmpty()){ TZMenu->addMenu(tmpC); }
    }
  }
  
}

void LClock::ThemeChange(){
  //TZMenu->setIcon(LXDG::findIcon("clock",""));

}*/

void LClock::OrientationChange(){
  if(this->layout()->direction()==QBoxLayout::LeftToRight){ //horizontal panel
    //button->setRotation(0); //no rotation of text
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  }else{  //vertical panel
    //button->setRotation(90); //90 degree rotation
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  }
  updateTime(true); //re-adjust the font/spacings
  this->layout()->update();
}
