#include "params.h"
#include <string.h>

//*** CParamList ***
CParamList::CParamList(CParamList *anext,const char *aName, const char *aValue, char aType) {
  Name=new char[strlen(aName)+1];
  strcpy(Name,aName);
  Value=new char[strlen(aValue)+1];
  strcpy(Value,aValue);
  Type=aType;
  changed=true;
  next=anext;
}

CParamList::~CParamList() {
  delete Name;
  delete Value;
}

CParamList *CParamList::GetParam(const char *aName) {
  if (strcmp(Name,aName)==0) return this;
  else if (next!=NULL) return next->GetParam(aName);
  else return NULL;
}

void CParamList::SetValue(const char* aValue) {
  if (strcmp(Value,aValue)==0) return;
  changed=true;
  delete Value;
  Value=new char[strlen(aValue)+1];
  strcpy(Value,aValue);
}

void CParamList::Save(ofstream &out, bool SaveAll, bool Update, bool Distance) {
  if (SaveAll || changed) {
    if (Distance) out<<"  ";
    out<<Name;
    if (Type!='S') out<<'='<<Value<<endl;
  }
  if (Update) changed=false;
  if (next!=NULL) next->Save(out, SaveAll, Update,Distance);
}

void CParamList::Update() {
  changed=false;
  if (next!=NULL) next->Update();
}


