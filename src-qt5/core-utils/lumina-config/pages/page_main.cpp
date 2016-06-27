//===========================================
//  Lumina Desktop Source Code
//  Copyright (c) 2016, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "page_main.h"
#include "ui_page_main.h"
#include "getPage.h"

//==========
//    PUBLIC
//==========
page_main::page_main(QWidget *parent) : PageWidget(parent), ui(new Ui::page_main()){
  ui->setupUi(this);
  ui->treeWidget->setMouseTracking(true);
  connect(ui->treeWidget, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(itemTriggered(QTreeWidgetItem*)) );
  connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(itemTriggered(QTreeWidgetItem*)) );
  connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(searchChanged(QString)) );
}

page_main::~page_main(){

}

void page_main::UpdateItems(QString search){
  ui->treeWidget->clear();
  //First create the categories
  QTreeWidgetItem *interface = new QTreeWidgetItem();
    interface->setIcon(0, LXDG::findIcon("preferences-desktop",""));
    interface->setText(0, tr("Interface Configuration"));
  QTreeWidgetItem *appearance = new QTreeWidgetItem();
    appearance->setIcon(0, LXDG::findIcon("preferences-desktop-color",""));
    appearance->setText(0, tr("Appearance"));
  QTreeWidgetItem *session = new QTreeWidgetItem();
    session->setIcon(0, LXDG::findIcon("preferences-system-session-services",""));
    session->setText(0, tr("Desktop Session Options"));
  QTreeWidgetItem *apps = new QTreeWidgetItem();
    apps->setIcon(0, LXDG::findIcon("preferences-desktop-filetype-association",""));
    apps->setText(0, tr("Application Settings"));
  //Now go through and add in the known pages for each category
  QStringList SL = search.split(" "); //search list
  for(int i=0; i<INFO.length(); i++){
    if(!search.isEmpty() ){
      //See if this item needs to be included or not
      QStringList info; info << INFO[i].name.split(" ") << INFO[i].title.split(" ") << INFO[i].comment.split(" ") << INFO[i].search_tags;
      info.removeDuplicates(); //remove any duplicate terms/info first
      bool ok = true;
      for(int s=0; s<SL.length() && ok; s++){
	ok = !info.filter(SL[s]).isEmpty();
      }
      if(!ok){ continue; } //no duplicates between search terms and available info
    }
    qDebug() << "Item Found:" << INFO[i].id << INFO[i].title;
    QTreeWidgetItem *it = new QTreeWidgetItem();
      it->setIcon(0, LXDG::findIcon(INFO[i].icon,"") );
      it->setText(0, INFO[i].name);
      it->setStatusTip(0, INFO[i].comment);
      it->setToolTip(0, INFO[i].comment);
      it->setWhatsThis(0, INFO[i].id);
    if(INFO[i].category=="interface"){ interface->addChild(it); }
    else if(INFO[i].category=="appearance"){ appearance->addChild(it); }
    else if(INFO[i].category=="session"){ session->addChild(it); }
    else if(INFO[i].category=="apps"){ apps->addChild(it); }
    else{ ui->treeWidget->addTopLevelItem(it); }
  }
  //Now add the categories to the tree widget if they are non-empty
  if(interface->childCount()>0){ ui->treeWidget->addTopLevelItem(interface); interface->setExpanded(true); }
  if(appearance->childCount()>0){ ui->treeWidget->addTopLevelItem(appearance); appearance->setExpanded(true); }
  if(session->childCount()>0){ ui->treeWidget->addTopLevelItem(session); session->setExpanded(true); }
  if(apps->childCount()>0){ ui->treeWidget->addTopLevelItem(apps); apps->setExpanded(true); }
}

//================
//    PUBLIC SLOTS
//================
void page_main::SaveSettings(){

}

void page_main::LoadSettings(int){
  emit HasPendingChanges(false);
  emit ChangePageTitle( tr("Desktop Settings") );
  INFO.clear();
  INFO = KnownPages();
  UpdateItems("");
}

void page_main::updateIcons(){
  UpdateItems("");
}

//=================
//    PRIVATE SLOTS
//=================
void page_main::itemTriggered(QTreeWidgetItem *it){
  if(it->childCount()>0){
    it->setExpanded( !it->isExpanded() ); 
  }else if(!it->whatsThis(0).isEmpty()){
    emit ChangePage(it->whatsThis(0));
  }
}

void page_main::searchChanged(QString txt){
  UpdateItems(txt.simplified());
}
