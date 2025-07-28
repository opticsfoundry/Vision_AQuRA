#include "params.h"
#include <string.h>
#include <stdio.h>
#include "leakdetect.h"

//*** CParamList ***
CParamList::CParamList(CParamList *anext,const char *aName, const char *aValue, char aType) {
  Name=new char[strlen(aName)+1];
  #if defined(DetectLeaks)
     LeakDetect.New(Name,9,1);
  #endif
  strcpy(Name,aName);
  Value=new char[strlen(aValue)+1];
  #if defined(DetectLeaks)
     LeakDetect.New(Value,9,2);
  #endif
  OldValue=NULL;
  strcpy(Value,aValue);
  Type=aType;
  changed=true;
  next=anext;
}

CParamList::~CParamList() {
  #if defined(DetectLeaks)
     LeakDetect.Delete(Name);
  #endif
  delete[] Name;
  #if defined(DetectLeaks)
     LeakDetect.Delete(Value);
  #endif
  delete[] Value;
  #if defined(DetectLeaks)
     LeakDetect.Delete(next);
  #endif
  if (next) delete next;
}


char *CParamList::GetName() {
   return Name;
}

char *CParamList::GetValue() {
   return Value;
}

char *CParamList::GetOldValue() {
   return OldValue;
}

char CParamList::GetType() {
  return Type;
}

bool CParamList::Getchanged() {
  return changed;
}

CParamList *CParamList::GetNext() {
  return next;
}

CParamList *CParamList::GetParam(const char *aName) {
  if (strcmp(Name,aName)==0) return this;
  else if (next!=NULL) return next->GetParam(aName);
  else return NULL;
}

void CParamList::SetValue(const char* aValue) {
  if (strcmp(Value,aValue)==0) return;
  changed=true;
  #if defined(DetectLeaks)
     LeakDetect.Delete(OldValue);
  #endif
  if (OldValue) delete OldValue;
  OldValue=Value;
  Value=new char[strlen(aValue)+1];
  #if defined(DetectLeaks)
     LeakDetect.New(Value,9,3);
  #endif
  strcpy(Value,aValue);
}

void CParamList::Save(ofstream &out, bool SaveAll, bool Update, bool Distance, TEdit* LogbookEdit, bool FirstLine, bool SkipUnused, bool Skipping) {
  // NO LONGER USED:   removed this function for saving the parameter data in order to fix the vision crashing problem due to iterative calling of a function to write each line of the parameter files - shayne 25/3/2017

  if (SaveAll || changed) {
    if ((Type=='T') && (!FirstLine)) {
      if (!Skipping) out<<endl;
      Skipping=false;
    }
    if (Type=='N') {
        if (!Skipping) out<<endl;
	    Skipping=false;
	} else {
        if ((Type=='F') && (SkipUnused)) {
		    Skipping=(strcmp(Value,"FALSE")==0);
	    }
		if (Distance) {
			if (!Skipping) out<<"  ";
			if (LogbookEdit) LogbookEdit->Insert("  ");
		}
		if (!Skipping) out<<Name;
		if (LogbookEdit) LogbookEdit->Insert(Name);
		if (Type!='T') {
			char lbuf[300];
			if ((changed) && (OldValue)) sprintf(lbuf,"=%s (%s)\r\n",Value,OldValue);
			else sprintf(lbuf,"=%s\r\n",Value);
			if (!Skipping) out<<lbuf;
			if (LogbookEdit) LogbookEdit->Insert(lbuf);
		} else {
			if (!Skipping) out<<endl;
			if (LogbookEdit) LogbookEdit->Insert("\r\n");
		}
     }
  }
  if (Update) changed=false;
  if (next!=NULL) next->Save(out, SaveAll, Update,Distance,LogbookEdit,false,SkipUnused, Skipping);


//do {
//
//  if (SaveAll || changed) {
//    if ((Type=='T') && (!FirstLine)) {
//      if (!Skipping) out<<endl;
//      Skipping=false;
//    }
//    if (Type=='N') {
//        if (!Skipping) out<<endl;
//	    Skipping=false;
//	} else {
//        if ((Type=='F') && (SkipUnused)) {
//		    Skipping=(strcmp(Value,"FALSE")==0);
//	    }
//		if (Distance) {
//			if (!Skipping) out<<"  ";
//			if (LogbookEdit) LogbookEdit->Insert("  ");
//		}
//		if (!Skipping) out<<Name;
//		if (LogbookEdit) LogbookEdit->Insert(Name);
//		if (Type!='T') {
//			char lbuf[300];
//			if ((changed) && (OldValue)) sprintf(lbuf,"=%s (%s)\r\n",Value,OldValue);
//			else sprintf(lbuf,"=%s\r\n",Value);
//			if (!Skipping) out<<lbuf;
//			if (LogbookEdit) LogbookEdit->Insert(lbuf);
//		} else {
//			if (!Skipping) out<<endl;
//			if (LogbookEdit) LogbookEdit->Insert("\r\n");
//		}
//     }
//  }
//  if (Update) changed=false;
//  FirstLine=false;
//} while (next!=NULL);
//  if (next!=NULL) next->Save(out, SaveAll, Update,Distance,LogbookEdit,false,SkipUnused, Skipping);

}

void CParamList::Update() {
//Causes crashed from recursive calling - eliminated and replaced by UpdateParameterList function
  changed=false;
  if (next!=NULL) next->Update();
}

void CParamList::SingleUpdate() {
  changed=false;
//  if (next!=NULL) next->Update();
}


