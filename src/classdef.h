/******************************************************************************
 *
 * $Id$
 *
 * Copyright (C) 1997-1999 by Dimitri van Heesch.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License is hereby 
 * granted. No representations are made about the suitability of this software 
 * for any purpose. It is provided "as is" without express or implied warranty.
 * See the GNU General Public License for more details.
 *
 * All output generated with Doxygen is not covered by this license.
 *
 */

#ifndef CLASSDEF_H
#define CLASSDEF_H

//#include <qtstream.h>
#include "qtbc.h"
#include <qfileinf.h>
#include <qlist.h>
#include <qdict.h>
#include <qstrlist.h>

#include "membername.h"
#include "memberlist.h"
#include "memberdef.h"
#include "example.h"
#include "entry.h"
#include "definition.h"

typedef QDict<MemberDef> MemberDict;
class ClassList;
class OutputList;
class FileDef;
class BaseClassList;
class MemberInfoList;
class MemberInfoDict;
class NamespaceDef;

class ClassDef : public Definition
{
  public:
    
    enum CompoundType { Class=Entry::CLASS_SEC, 
                        Struct=Entry::STRUCT_SEC, 
                        Union=Entry::UNION_SEC,
                        Interface=Entry::INTERFACE_SEC
                      };
    
    ClassDef(const char *name,CompoundType ct,const char *ref=0,const char *fName=0);
   ~ClassDef();
    //QCString classFile() const { return fileName; }
    QCString getOutputFileBase() const { return fileName; }
    CompoundType compoundType() const { return compType; } 
    //const char *memberListFileName() const { return memListFileName; }
    void insertBaseClass(ClassDef *,Protection p,Specifier s,const char *t=0);
    BaseClassList *baseClasses() { return inherits; }
    void insertSuperClass(ClassDef *,Protection p,Specifier s,const char *t=0);
    BaseClassList *superClasses() { return inheritedBy; }
    void setIncludeFile(FileDef *fd) { incFile=fd; }
    FileDef *includeFile() const { return incFile; }
    void setIncludeName(const char *n_) { incName=n_; }
    MemberNameInfoList *memberNameInfoList() { return allMemberNameInfoList; }
    MemberNameInfoDict *memberNameInfoDict() { return allMemberNameInfoDict; }
    //bool isReference() { return !reference.isNull(); }
    //const char *getReference() const { return reference; }
    void insertMember(const MemberDef *);
    void insertUsedFile(const char *);
    void computeAnchors();
    void setAnchor(MemberDef *);
    void dumpMembers();
    void writeDocumentation(OutputList &ol);
    void writeMemberList(OutputList &ol);
    void writeIncludeFile(OutputList &ol);
    //void writeMembersToContents();
    void writeDeclaration(OutputList &ol);
    bool addExample(const char *anchor,const char *name, const char *file);
    bool hasExamples();
    //void writeExample(OutputList &ol);
    void setProtection(Protection p) { prot=p; }
    Protection protection() const { return prot; }
    /*! a link to this class is possible within this project */
    bool isLinkableInProject() 
    { int i = name().findRev("::");
      if (i==-1) i=0; else i+=2;
      return !name().isEmpty() && name().at(i)!='@' && 
             (prot!=Private || Config::extractPrivateFlag) &&
             hasDocumentation() && !isReference();
    }
    /*! a link to this class is possible (either within this project,
     *  or as a cross-reference to another project
     */
    bool isLinkable()
    {
      return isLinkableInProject() || isReference();
    }
    bool hasNonReferenceSuperClass();
    /*! the class is visible in a class diagram, or class hierarchy */
    bool isVisibleInHierarchy() 
    { return // show all classes or a superclass is visible
             (Config::allExtFlag || hasNonReferenceSuperClass()) &&
             // and not an annonymous compound
             name().find('@')==-1 &&
             // and not privately inherited
             (prot!=Private || Config::extractPrivateFlag) &&
             // documented or show anyway or documentation is external 
             (hasDocumentation() || !Config::hideClassFlag || isReference());
    }
    
    // template argument functions
    ArgumentList *templateArguments() const { return tempArgs; }
    void setTemplateArguments(ArgumentList *al);
    //QCString getTemplateNameString();
    void setNamespace(NamespaceDef *nd) { nspace = nd; }
    NamespaceDef *getNamespace() { return nspace; }
    
    bool visited;
   
  private: 
    //QCString name;                       // name of the class
    QCString fileName;                   // HTML containing the class docs
    //QCString doc;                        // general class documentation
    FileDef *incFile;                   // header file to refer to
    QCString incName;                    // alternative include file name
    //QCString brief;                      // brief class discription
    QCString memListFileName;            
    BaseClassList *inherits;
    BaseClassList *inheritedBy;
    NamespaceDef  *nspace;              // the namespace this class is in
    MemberList pubMembers;
    MemberList proMembers;
    MemberList priMembers;
    MemberList pubStaticMembers;
    MemberList proStaticMembers;
    MemberList priStaticMembers;
    MemberList pubSlots;
    MemberList proSlots;
    MemberList priSlots;
    MemberList related;
    MemberList signals;
    MemberList friends;
    MemberNameInfoList *allMemberNameInfoList;
    MemberNameInfoDict *allMemberNameInfoDict;
    ArgumentList     *tempArgs;
    QStrList          files;
    //QCString           reference;
    ExampleList      *exampleList;
    ExampleDict      *exampleDict;
    CompoundType      compType;
    Protection        prot;
};


struct BaseClassDef
{
  BaseClassDef(ClassDef *cd,Protection p,Specifier v,const char *t) : 
        classDef(cd), prot(p), virt(v), templSpecifiers(t) {}
  ClassDef *classDef;
  Protection prot;
  Specifier  virt;
  QCString templSpecifiers;
};

class BaseClassList : public QList<BaseClassDef>
{
  public:
    int compareItems(GCI item1,GCI item2)
    {
      ClassDef *c1=((BaseClassDef *)item1)->classDef;
      ClassDef *c2=((BaseClassDef *)item2)->classDef;
      if (c1==0 || c2==0) 
        return FALSE;
      else
        return strcmp(c1->name(),c2->name());
    }
};

class BaseClassListIterator : public QListIterator<BaseClassDef>
{
  public:
    BaseClassListIterator(const BaseClassList &bcl) : 
      QListIterator<BaseClassDef>(bcl) {}
};

#endif
