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

#include <stdlib.h>

#include "qtbc.h"
#include <qdir.h>
#include "message.h"
#include "htmlgen.h"
#include "config.h"
#include "util.h"
#include "doxygen.h"
#include "logos.h"
#include "diagram.h"

#define GROUP_COLOR "#ff8080"

HtmlHelp *HtmlGenerator::htmlHelp = 0;

HtmlGenerator::HtmlGenerator() : OutputGenerator()
{
  if (Config::headerFile.length()>0) header=fileToString(Config::headerFile);
  if (Config::footerFile.length()>0) footer=fileToString(Config::footerFile);
  dir=Config::htmlOutputDir;
}

HtmlGenerator::~HtmlGenerator()
{
}

void HtmlGenerator::append(const OutputGenerator *g)
{
  t << g->getContents();
}

void HtmlGenerator::init()
{
  QDir d(Config::htmlOutputDir);
  if (!d.exists() && !d.mkdir(Config::htmlOutputDir))
  {
    err("Could not create output directory %s\n",Config::htmlOutputDir.data());
    exit(1);
  }
  writeLogo(Config::htmlOutputDir);
  writeNullImage(Config::htmlOutputDir);
}

void HtmlGenerator::startFile(const char *name,const char *title,bool external)
{
  QCString fileName=name;
  lastTitle=title;
  if (fileName.right(5)!=".html") fileName+=".html";
  startPlainFile(fileName);
  lastFile = fileName;
  if (header.length()==0) 
  {
    t << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n"
         "<html><head><meta name=\"robots\" content=\"noindex\">\n"
         "<title>" << title << "</title>\n";
    t << "<link ";
    if (external) 
      t << "doxygen=\"_doc:\" href=\"/";
    else
      t << "href=\"";
    t << "doxygen.css\" rel=\"stylesheet\" type=\"text/css\">\n"
         "</head><body bgcolor=\"#ffffff\">\n";
  }
  else
  {
    t << substitute(
           substitute(  
             substitute(header,"$title",lastTitle),
             "$datetime",dateToString(TRUE)
           ),
           "$date",dateToString(FALSE)
         );
  }
}

void HtmlGenerator::startQuickIndexItem(const char *s,const char *l)
{
  t << "<a ";
  if (s) t << "doxygen=\"" << s << "\" ";
  t << "href=\"" << l << "\">";
}

void HtmlGenerator::endQuickIndexItem()
{
  t << "</a> &nbsp; ";
}

void HtmlGenerator::writeFooter(int part,bool external)
{
  switch (part)
  {
    case 0:
      if (footer.length()==0)
        t << "<hr><address><small>";
      else
        t << substitute(
               substitute(  
                 substitute(footer,"$title",lastTitle),
                 "$datetime",dateToString(TRUE)
               ),
               "$date",dateToString(FALSE)
             );
      break;
    case 1:
      if (footer.length()==0)
      {
        t << " <a href=\"http://www.stack.nl/~dimitri/doxygen/index.html\">";
        t << "<img ";
        if (external)
        {
          t << "doxygen=\"_doc:\" src=\"/";
        }
        else
        {
          t << "src=\"";
        }
        t << "doxygen.gif\" alt=\"doxygen\" align=center border=0 "
             "width=118 height=53></a>&nbsp; ";
      }
      break;
    default:
      if (footer.length()==0)
        t << " <a href=\"mailto:dimitri@stack.nl\">Dimitri van Heesch</a>, &copy; "
             "1997-1999</small></address>\n</body>\n</html>\n";
      break;
      
  }
}

void HtmlGenerator::endFile()
{
  endPlainFile();
}

void HtmlGenerator::startProjectNumber()
{
  t << "<h3 align=center>";
}

void HtmlGenerator::endProjectNumber()
{
  t << "</h3>";
}

void HtmlGenerator::writeStyleInfo(int part)
{
  if (part==0)
  {
    startPlainFile("doxygen.css"); 
      //<< "H1 { border-width: thin; border: solid; text-align: center }" << endl
    t << "H1 { text-align: center }" << endl;
    t << "A.el { text-decoration: none; font-weight: bold }" << endl;
    t << "DL.el { margin-left: -1cm }" << endl;
    t << "DIV.fragment { width: 100%; border: none; background-color: #eeeeee }" << endl;
    t << "DIV.in { margin-left: 16 }" << endl;
    t << "A.gl:link { color: #ffffff }" << endl;
    t << "A.gl:visited { color: #ffffff }" << endl;
    t << "A.gl { text-decoration: none; font-weight: bold; background-color: " << GROUP_COLOR << " }" << endl;
    t << endl;
    endPlainFile();
  }
}

void HtmlGenerator::writeDoxyAnchor(const char *,const char *anchor, const char *name)
{
  t << "<a name=\"" << anchor << "\" doxytag=\"" << name << "\"></a>";
}

void HtmlGenerator::newParagraph()
{
  t << endl << "<p>" << endl;
}

void HtmlGenerator::writeString(const char *text)
{
  t << text;
}

void HtmlGenerator::writeIndexItem(const char *ref,const char *f,
                                   const char *name)
{
  t << "<li>";
  if (ref || f)
  {
    t << "<a class=\"el\" ";
    if (ref) t << "doxygen=\"" << ref << ":\" ";
    t << "href=\"";
    if (f) t << f << ".html\">";
  }
  else
  {
    t << "<b>";
  }
  docify(name);
  if (ref || f)
  {
    t << "</a>" << endl;
  }
  else
  {
    t << "</b>";
  }
  //if (Config::htmlHelpFlag && f)
  //{
  //  htmlHelp->addItem(name,((QCString)f)+".html");
  //}
}

void HtmlGenerator::writeStartAnnoItem(const char *,const char *f,
                                       const char *path,const char *name)
{
  t << "<li>";
  if (path) docify(path);
  t << "<a class=\"el\" href=\"" << f << ".html\">";
  docify(name);
  t << "</a> ";
  //if (Config::htmlHelpFlag && f)
  //{
  //  htmlHelp->addItem(name, ((QCString)f)+".html");
  //}
}

void HtmlGenerator::writeObjectLink(const char *ref,const char *f,
                                    const char *anchor, const char *name)
{
  t << "<a class=\"el\" ";
  if (ref) t << "doxygen=\"" << ref << ":\" ";
  t << "href=\"";
  if (f) t << f << ".html";
  if (anchor) t << "#" << anchor;
  t << "\">";
  docify(name);
  t << "</a>";
  //if (Config::htmlHelpFlag && f && htmlHelp->depth()>0)
  //{
  //  htmlHelp->addItem(name,((QCString)f)+".html");
  //}
}

void HtmlGenerator::startTextLink(const char *f,const char *anchor)
{
  t << "<a href=\"";
  if (f)   t << f << ".html";
  if (anchor) t << "#" << anchor;
  t << "\">"; 
}

void HtmlGenerator::endTextLink()
{
  t << "</a>";
}

void HtmlGenerator::writeHtmlLink(const char *url,const char *text)
{
  t << "<a href=\"";
  if (url) t << url;
  t << "\">"; 
  docify(text);
  t << "</a>";
}

void HtmlGenerator::startGroupHeader()
{
  t << "<h2>";
}

void HtmlGenerator::endGroupHeader()
{
  t << "</h2>" << endl;
}

void HtmlGenerator::writeSection(const char *lab,const char *title,bool sub)
{
  t << "<a name=\"" << lab << "\">";
  if (sub) t << "<h3>"; else t << "<h2>";
  docify(title);
  if (sub) t << "</h3>"; else t << "</h2>";
  t << "</a>" << endl;
}

void HtmlGenerator::writeSectionRef(const char *name,const char *lab,
                                    const char *title)
{
  QCString refName=name;
  if (refName.right(5)!=".html") refName+=".html";
  t << "<a href=\"" << refName << "#" << lab << "\">";
  docify(title);
  t << "</a>";
}

void HtmlGenerator::writeSectionRefItem(const char *name,const char *lab,
                                    const char *title)
{
  QCString refName=name;
  if (refName.right(5)!=".html") refName+=".html";
  t << "<a href=\"" << refName << "#" << lab << "\">";
  docify(title);
  t << "</a>";
}

void HtmlGenerator::writeSectionRefAnchor(const char *name,const char *lab,
                                          const char *title)
{
  writeSectionRef(name,lab,title);
}

void HtmlGenerator::docify(const char *str)
{
  if (str)
  {
    const char *p=str;
    char c;
    while (*p)
    {
      c=*p++;
      switch(c)
      {
        case '<':  t << "&lt;"; break;
        case '>':  t << "&gt;"; break;
        case '&':  t << "&amp;"; break;
        case '\\':
                   if (*p=='<')
                     { t << "&lt;"; p++; }
                   else if (*p=='>')
                     { t << "&gt;"; p++; }
                   else
                     t << "\\";
                   break;
        default:   t << c; 
      }
    }
  }
}

void HtmlGenerator::codify(const char *str)
{
  docify(str);
}

void HtmlGenerator::writeChar(char c)
{
  char cs[2];
  cs[0]=c;
  cs[1]=0;
  docify(cs);
}

void HtmlGenerator::startClassDiagram()
{
  t << "<p>";
}

void HtmlGenerator::endClassDiagram(ClassDiagram &d,
                                const char *fileName,const char *name)
{
  t << ":\n<p><center><img src=\""
    << fileName << ".gif\" usemap=\"#" << name << "_map\""
    << " border=\"0\"></center>" << endl
    << "<map name=\"" << name << "_map\"><p>" << endl;

  d.writeImageMap(t,dir,fileName);
}

void HtmlGenerator::startColorFont(uchar red,uchar green,uchar blue)
{
  QCString colorString;
  colorString.sprintf("%02x%02x%02x",red,green,blue);
  t << "<font color=\"#" << colorString << "\">";
}

void HtmlGenerator::endColorFont()
{
  t << "</font>";
}

void HtmlGenerator::writeFormula(const char *n,const char *text)
{
  if (text && text[0]=='\\') t << "<p><center>" << endl;
  t << "<img align=\"top\" src=\"" << n << "\">" << endl;
  if (text && text[0]=='\\') t << "</center><p>" << endl;
}

void HtmlGenerator::startMemberList()  
{ 
  if (Config::htmlAlignMemberFlag)
  {
    //t << endl << "<p><table border=0 cellspacing=2 cellpadding=0>" << endl; 
  }
  else
  {
    t << "<ul>" << endl; 
  }
}

void HtmlGenerator::endMemberList()    
{ 
  if (Config::htmlAlignMemberFlag)
  {
    //t << "</table>" << endl; 
  }
  else
  {
    t << "</ul>" << endl; 
  }
}

// annonymous type:
//  0 = single column right aligned
//  1 = double column left aligned
//  2 = single column left aligned
void HtmlGenerator::startMemberItem(bool inGroup,int annoType) 
{ 
  if (Config::htmlAlignMemberFlag)
  {
    t << "<tr>";
    if (inGroup)
      t << "<td bgcolor=\"" << GROUP_COLOR << "\">";
    else
      t << "<td>";
    t << "<img src=\"null.gif\"></td><td><img src=\"null.gif\"></td>";
    switch(annoType)
    {
      case 0:  t << "<td nowrap align=right valign=top>"; break;
      case 1:  t << "<td nowrap>"; break;
      default: t << "<td nowrap valign=top>"; break;
    }
  }
  else
  {
    t << "<li>"; 
  }
}

void HtmlGenerator::endMemberItem(bool inGroup,
                          const char *fileName,const char *headerName,bool) 
{ 
  if (Config::htmlAlignMemberFlag)
  {
    if (inGroup)
    {
      t << "&nbsp;</td><td";
      if (headerName)
      {
        t << " align=right valign=top><a class=\"gl\" href=\"" 
          << fileName << ".html\">&nbsp;" << headerName << "&nbsp;</a>";
      }
      else
      {
        t << ">";
      }
      t << "</td><td bgcolor=\"" << GROUP_COLOR 
        << "\"><img src=\"null.gif\">";
    }
    t << "</td></tr>"; 
  }
  t << endl; 
}

void HtmlGenerator::insertMemberAlign() 
{ 
  if (Config::htmlAlignMemberFlag)
  {
    t << "&nbsp;</td><td valign=top>"; 
  }
}

void HtmlGenerator::startMemberDescription() 
{ 
  if (Config::htmlAlignMemberFlag)
  {
    t << "<tr><td><img src=null.gif></td><td><img src=null.gif></td>"
         "<td></td><td><font size=-1><em>"; 
  }
  else
  {
    t << "<dl class=\"el\"><dd><font size=-1><em>";
  }
}

void HtmlGenerator::endMemberDescription()   
{ 
  if (Config::htmlAlignMemberFlag)
  {
    t << "</em></font><br><br></td></tr>" << endl; 
  }
  else
  {
    t << "</em></font></dl>";
  }
}

void HtmlGenerator::startMemberSections()
{
  if (Config::htmlAlignMemberFlag)
  {
    t << "<table border=0 cellpadding=0 cellspacing=0>" << endl;
  }
}

void HtmlGenerator::endMemberSections()
{
  if (Config::htmlAlignMemberFlag)
  {
    t << "</table>" << endl;
  }
}

void HtmlGenerator::startMemberHeader()
{
  if (Config::htmlAlignMemberFlag)
  {
    t << "<tr><td colspan=4><br><h2>";
  }
  else
  {
    startGroupHeader();
  }
}

void HtmlGenerator::endMemberHeader()
{
  if (Config::htmlAlignMemberFlag)
  {
    t << "</h2></td></tr>" << endl;
  }
  else
  {
    endGroupHeader();
  }
}

void HtmlGenerator::memberGroupSpacing(bool inGroup)
{
  t << "<tr><td";
  if (inGroup)
  {
         // left vertical table line
    t << " bgcolor=\"" << GROUP_COLOR << "\" height=1><img src=\"null.gif\">"
         // white space
         "</td><td colspan=4></td>"
         // right vertical table line
         "<td bgcolor=\"" << GROUP_COLOR << "\"><img src=\"null.gif\">";
  }
  else
  {
         // one pixel height of whitespace
    t << " height=1>";
  }
  t << "</td></tr>" << endl;
}

void HtmlGenerator::memberGroupSeparator()
{
  t << "<tr><td colspan=6 height=1 bgcolor=\"" << GROUP_COLOR 
    << "\"><img src=\"null.gif\"></td></tr>" << endl;
}

void HtmlGenerator::startMemberSubtitle()
{
  if (Config::htmlAlignMemberFlag) t << "<tr><td colspan=4>";
}

void HtmlGenerator::endMemberSubtitle()
{
  if (Config::htmlAlignMemberFlag) t << "<br><br></td></tr>" << endl;
}

void HtmlGenerator::startIndexList() 
{ 
  t << "<ul>"  << endl; 
  //if (Config::htmlHelpFlag)
  //{
  //  if (htmlHelp->depth()==0) htmlHelp->addItem(lastTitle,lastFile);
  //  htmlHelp->incDepth();
  //}
}

void HtmlGenerator::endIndexList()
{
  t << "</ul>" << endl;
  //if (Config::htmlHelpFlag)
  //{
  //  htmlHelp->decDepth();
  //}
}

void HtmlGenerator::startAlfabeticalIndexList()
{
  t << "<multicol cols=5><dl compact>" << endl;
}

void HtmlGenerator::endAlfabeticalIndexList()
{
  t << "</dl></multicol>" << endl;
}

void HtmlGenerator::writeIndexHeading(const char *s)
{
  t << "<dt><b><big>" << s << "</big></b><dd>" << endl;
}
