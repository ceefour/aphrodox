/******************************************************************************
 *
 * 
 *
 *
 * Copyright (C) 1997-2005 by Dimitri van Heesch.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License is hereby 
 * granted. No representations are made about the suitability of this software 
 * for any purpose. It is provided "as is" without express or implied warranty.
 * See the GNU General Public License for more details.
 *
 * Documents produced by Doxygen are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
 */

#include "rtfdocvisitor.h"
#include "docparser.h"
#include "language.h"
#include "doxygen.h"
#include "outputgen.h"
#include "code.h"
#include "dot.h"
#include "util.h"
#include "rtfstyle.h"
#include "message.h"
#include <qfileinfo.h> 


#define DBG_RTF(x) m_t << x
//#define DBG_RTF(x) do {} while(0)

RTFDocVisitor::RTFDocVisitor(QTextStream &t,BaseCodeDocInterface &ci) 
  : DocVisitor(DocVisitor_RTF), m_t(t), m_ci(ci), m_insidePre(FALSE), 
    m_hide(FALSE), m_indentLevel(0), m_lastIsPara(FALSE)
{
}

QString RTFDocVisitor::getStyle(const char *name)
{
  QString n;
  n.sprintf("%s%d",name,m_indentLevel);
  StyleData *sd = rtf_Style[n];
  ASSERT(sd!=0);
  return sd->reference;
}

void RTFDocVisitor::incIndentLevel()
{
  if (m_indentLevel<rtf_maxIndentLevels-1) m_indentLevel++;
}

void RTFDocVisitor::decIndentLevel()
{
  if (m_indentLevel>0) m_indentLevel--;
}

  //--------------------------------------
  // visitor functions for leaf nodes
  //--------------------------------------

void RTFDocVisitor::visit(DocWord *w)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocWord)}\n");
  filter(w->word());
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visit(DocLinkedWord *w)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocLinkedWord)}\n");
  startLink(w->ref(),w->file(),w->anchor());
  filter(w->word());
  endLink(w->ref());
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visit(DocWhiteSpace *w)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocWhiteSpace)}\n");
  if (m_insidePre)
  {
    m_t << w->chars();
  }
  else
  {
    m_t << " ";
  }
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visit(DocSymbol *s)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocSymbol)}\n");
  switch(s->symbol())
  {
    case DocSymbol::BSlash:  m_t << "\\\\"; break;
    case DocSymbol::At:      m_t << "@"; break;
    case DocSymbol::Less:    m_t << "<"; break;
    case DocSymbol::Greater: m_t << ">"; break;
    case DocSymbol::Amp:     m_t << "&"; break;
    case DocSymbol::Dollar:  m_t << "$"; break;
    case DocSymbol::Hash:    m_t << "#"; break;
    case DocSymbol::Percent: m_t << "%"; break;
    case DocSymbol::Copy:    m_t << "(C)"; break;
    case DocSymbol::Tm:      m_t << "(TM)"; break;
    case DocSymbol::Reg:     m_t << "(R)"; break;
    case DocSymbol::Apos:    m_t << "'"; break;
    case DocSymbol::Quot:    m_t << "\""; break;
    case DocSymbol::Uml:     switch(s->letter())
                             {
                               case 'A' : m_t << '\304'; break;
                               case 'E' : m_t << '\313'; break;
                               case 'I' : m_t << '\317'; break;
                               case 'O' : m_t << '\326'; break;
                               case 'U' : m_t << '\334'; break;
                               case 'Y' : m_t << 'Y';    break;
                               case 'a' : m_t << '\344'; break;
                               case 'e' : m_t << '\353'; break;
                               case 'i' : m_t << '\357'; break;
                               case 'o' : m_t << '\366'; break;
                               case 'u' : m_t << '\374'; break;
                               case 'y' : m_t << '\377'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Acute:   switch(s->letter())
                             {
                               case 'A' : m_t << '\301'; break;
                               case 'E' : m_t << '\311'; break;
                               case 'I' : m_t << '\315'; break;
                               case 'O' : m_t << '\323'; break;
                               case 'U' : m_t << '\332'; break;
                               case 'Y' : m_t << '\335'; break;
                               case 'a' : m_t << '\341'; break;
                               case 'e' : m_t << '\351'; break;
                               case 'i' : m_t << '\355'; break;
                               case 'o' : m_t << '\363'; break;
                               case 'u' : m_t << '\372'; break;
                               case 'y' : m_t << '\375'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Grave:   switch(s->letter())
                             {
                               case 'A' : m_t << '\300'; break;
                               case 'E' : m_t << '\310'; break;
                               case 'I' : m_t << '\314'; break;
                               case 'O' : m_t << '\322'; break;
                               case 'U' : m_t << '\331'; break;
                               case 'a' : m_t << '\340'; break;
                               case 'e' : m_t << '\350'; break;
                               case 'i' : m_t << '\354'; break;
                               case 'o' : m_t << '\362'; break;
                               case 'u' : m_t << '\371'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Circ:    switch(s->letter())
                             {
                               case 'A' : m_t << '\302'; break;
                               case 'E' : m_t << '\312'; break;
                               case 'I' : m_t << '\316'; break;
                               case 'O' : m_t << '\324'; break;
                               case 'U' : m_t << '\333'; break;
                               case 'a' : m_t << '\342'; break;
                               case 'e' : m_t << '\352'; break;
                               case 'i' : m_t << '\356'; break;
                               case 'o' : m_t << '\364'; break;
                               case 'u' : m_t << '\373'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Tilde:   switch(s->letter())
                             {
                               case 'A' : m_t << '\303'; break;
                               case 'N' : m_t << '\321'; break;
                               case 'O' : m_t << '\325'; break;
                               case 'a' : m_t << '\343'; break;
                               case 'n' : m_t << '\361'; break;
                               case 'o' : m_t << '\365'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Cedil:   switch(s->letter())
                             {
                               case 'C' : m_t << '\307'; break;
                               case 'c' : m_t << '\347'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Slash:   switch(s->letter())
                             {
                               case 'O' : m_t << '\330'; break;
                               case 'o' : m_t << '\370'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Ring:    switch(s->letter())
                             {
                               case 'A' : m_t << '\305'; break;
                               case 'a' : m_t << '\345'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Szlig:   m_t << "\337"; break;
    case DocSymbol::Nbsp:    m_t << "\\~ "; break;
    default:
                             err("Error: unknown symbol found\n");
  }
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visit(DocURL *u)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocURL)}\n");
  if (Config_getBool("RTF_HYPERLINKS"))
  {
    m_t << "{\\field "
             "{\\*\\fldinst "
               "{ HYPERLINK  \\\\l \"";
    if (u->isEmail()) m_t << "mailto:";
    m_t << u->url();
    m_t <<  "\" }"
               "{}";
    m_t <<   "}"
             "{\\fldrslt "
               "{\\cs37\\ul\\cf2 ";
    filter(u->url());
    m_t <<     "}"
             "}"
           "}" << endl;
  }
  else
  {
    m_t << "{\\f2 ";
    filter(u->url());
    m_t << "}";
  }
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visit(DocLineBreak *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocLineBreak)}\n");
  m_t << "\\par\n";
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visit(DocHorRuler *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocHorRuler)}\n");
  m_t << "{\\pard\\widctlpar\\brdrb\\brdrs\\brdrw5\\brsp20 \\adjustright \\par}" << endl;
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visit(DocStyleChange *s)
{
  if (m_hide) return;
  m_lastIsPara=FALSE;
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocStyleChange)}\n");
  switch (s->style())
  {
    case DocStyleChange::Bold:
      if (s->enable()) m_t << "{\\b ";      else m_t << "} ";
      break;
    case DocStyleChange::Italic:
      if (s->enable()) m_t << "{\\i ";     else m_t << "} ";
      break;
    case DocStyleChange::Code:
      if (s->enable()) m_t << "{\\f2 ";   else m_t << "} ";
      break;
    case DocStyleChange::Subscript:
      if (s->enable()) m_t << "{\\sub ";    else m_t << "} ";
      break;
    case DocStyleChange::Superscript:
      if (s->enable()) m_t << "{\\super ";    else m_t << "} ";
      break;
    case DocStyleChange::Center:
      if (s->enable()) m_t << "{\\qc "; else m_t << "} ";
      break;
    case DocStyleChange::Small:
      if (s->enable()) m_t << "{\\sub ";  else m_t << "} ";
      break;
    case DocStyleChange::Preformatted:
      if (s->enable())
      {
        m_t << "{" << endl;
        m_t << "\\par" << endl;
        m_t << rtf_Style_Reset << getStyle("CodeExample");
        m_insidePre=TRUE;
      }
      else
      {
        m_insidePre=FALSE;
        m_t << "\\par" << endl; 
        m_t << "}" << endl;
      }
      m_lastIsPara=TRUE;
      break;
    case DocStyleChange::Div:  /* HTML only */ break;
    case DocStyleChange::Span: /* HTML only */ break;
  }
}

void RTFDocVisitor::visit(DocVerbatim *s)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocVerbatim)}\n");
  switch(s->type())
  {
    case DocVerbatim::Code: // fall though
      m_t << "{" << endl;
      m_t << "\\par" << endl;
      m_t << rtf_Style_Reset << getStyle("CodeExample");
      parseCode(m_ci,s->context(),s->text().latin1(),s->isExample(),s->exampleFile());
      //m_t << "\\par" << endl; 
      m_t << "}" << endl;
      break;
    case DocVerbatim::Verbatim: 
      m_t << "{" << endl;
      m_t << "\\par" << endl;
      m_t << rtf_Style_Reset << getStyle("CodeExample");
      filter(s->text(),TRUE);
      //m_t << "\\par" << endl; 
      m_t << "}" << endl;
      break;
    case DocVerbatim::HtmlOnly: 
    case DocVerbatim::LatexOnly: 
    case DocVerbatim::XmlOnly: 
    case DocVerbatim::ManOnly: 
      /* nothing */
      break;
    case DocVerbatim::Dot: 
      {
        static int dotindex = 1;
        QCString fileName(4096);

        fileName.sprintf("%s%d", 
            (Config_getString("RTF_OUTPUT")+"/inline_dotgraph_").data(), 
            dotindex++
           );
        QFile file(fileName);
        if (!file.open(IO_WriteOnly))
        {
          err("Could not open file %s for writing\n",fileName.data());
        }
        file.writeBlock( s->text(), s->text().length() );
        file.close();
        m_t << "\\par{\\qc "; // center picture
        writeDotFile(fileName);
        m_t << "} ";
        file.remove();
      }
      break;
  }
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visit(DocAnchor *anc)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocAnchor)}\n");
  QString anchor;
  if (!anc->file().isEmpty())
  {
    anchor+=anc->file();
  }
  if (!anc->file().isEmpty() && !anc->anchor().isEmpty())
  {
    anchor+="_";
  }
  if (!anc->anchor().isEmpty())
  {
    anchor+=anc->anchor();
  }
  m_t << "{\\bkmkstart " << rtfFormatBmkStr(anchor) << "}" << endl;
  m_t << "{\\bkmkend " << rtfFormatBmkStr(anchor) << "}" << endl;
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visit(DocInclude *inc)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocInclude)}\n");
  switch(inc->type())
  {
   case DocInclude::IncWithLines:
      { 
         m_t << "{" << endl;
         m_t << "\\par" << endl;
         m_t << rtf_Style_Reset << getStyle("CodeExample");
         QFileInfo cfi( inc->file() );
         FileDef fd( cfi.dirPath(), cfi.fileName() );
         parseCode(m_ci,inc->context(),inc->text().latin1(),inc->isExample(),inc->exampleFile(), &fd);
         m_t << "\\par" << endl; 
         m_t << "}" << endl;
      }
      break;
   case DocInclude::Include: 
      m_t << "{" << endl;
      m_t << "\\par" << endl;
      m_t << rtf_Style_Reset << getStyle("CodeExample");
      parseCode(m_ci,inc->context(),inc->text().latin1(),inc->isExample(),inc->exampleFile());
      m_t << "\\par" << endl; 
      m_t << "}" << endl;
      break;
    case DocInclude::DontInclude: 
      break;
    case DocInclude::HtmlInclude: 
      break;
    case DocInclude::VerbInclude: 
      m_t << "{" << endl;
      m_t << "\\par" << endl;
      m_t << rtf_Style_Reset << getStyle("CodeExample");
      filter(inc->text());
      m_t << "\\par" << endl; 
      m_t << "}" << endl;
      break;
  }
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visit(DocIncOperator *op)
{
  //printf("DocIncOperator: type=%d first=%d, last=%d text=`%s'\n",
  //    op->type(),op->isFirst(),op->isLast(),op->text().data());
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocIncOperator)}\n");
  if (op->isFirst()) 
  {
    if (!m_hide)
    {
      m_t << "{" << endl;
      m_t << "\\par" << endl;
      m_t << rtf_Style_Reset << getStyle("CodeExample");
    }
    pushEnabled();
    m_hide = TRUE;
  }
  if (op->type()!=DocIncOperator::Skip) 
  {
    popEnabled();
    if (!m_hide) parseCode(m_ci,op->context(),op->text().latin1(),op->isExample(),op->exampleFile());
    pushEnabled();
    m_hide=TRUE;
  }
  if (op->isLast())  
  {
    popEnabled();
    if (!m_hide)
    {
      m_t << "\\par" << endl; 
      m_t << "}" << endl;
    }
    m_lastIsPara=TRUE;
  }
  else
  {
    if (!m_hide) m_t << endl;
    m_lastIsPara=FALSE;
  }
}

void RTFDocVisitor::visit(DocFormula *f)
{
  if (m_hide) return;
  // TODO: do something sensible here, like including a bitmap
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocFormula)}\n");
  m_t << f->text();
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visit(DocIndexEntry *i)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visit(DocIndexEntry)}\n");
  m_t << "{\\xe \\v " << i->entry() << "}" << endl;
  m_lastIsPara=FALSE;
}

//--------------------------------------
// visitor functions for compound nodes
//--------------------------------------

void RTFDocVisitor::visitPre(DocAutoList *l)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocAutoList)}\n");
  m_t << "{" << endl;
  rtf_listItemInfo[m_indentLevel].isEnum = l->isEnumList();
  rtf_listItemInfo[m_indentLevel].number = 1;
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocAutoList *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocAutoList)}\n");
  m_t << "\\par" << endl;
  m_t << "}" << endl;
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocAutoListItem *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocAutoListItem)}\n");
  if (!m_lastIsPara) m_t << "\\par" << endl;
  m_t << rtf_Style_Reset;
  if (rtf_listItemInfo[m_indentLevel].isEnum)
  {
    m_t << getStyle("ListEnum") << endl;
    m_t << rtf_listItemInfo[m_indentLevel].number << ".\\tab ";
    rtf_listItemInfo[m_indentLevel].number++;
  }
  else
  {
    m_t << getStyle("ListBullet") << endl;
  }
  incIndentLevel();
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocAutoListItem *) 
{
  decIndentLevel();
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocAutoListItem)}\n");
}

void RTFDocVisitor::visitPre(DocPara *) 
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocPara)}\n");
}

void RTFDocVisitor::visitPost(DocPara *p)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocPara)}\n");
  if (!p->isLast() &&            // omit <p> for last paragraph
      !(p->parent() &&           // and for parameter sections
        p->parent()->kind()==DocNode::Kind_ParamSect
       )
     ) 
  {
    m_t << "\\par" << endl;
    m_lastIsPara=TRUE;
  }
}

void RTFDocVisitor::visitPre(DocRoot *r)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocRoot)}\n");
  if (r->indent()) incIndentLevel();
  m_t << "{" << rtf_Style["BodyText"]->reference << endl;
}

void RTFDocVisitor::visitPost(DocRoot *r)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocRoot)}\n");
  if (!m_lastIsPara && !r->singleLine()) m_t << "\\par" << endl;
  m_t << "}";
  m_lastIsPara=TRUE;
  if (r->indent()) decIndentLevel();
}

void RTFDocVisitor::visitPre(DocSimpleSect *s)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocSimpleSect)}\n");
  if (!m_lastIsPara) m_t << "\\par" << endl;
  m_t << "{"; // start desc
  //m_t << "{\\b "; // start bold
  m_t << "{" << rtf_Style["Heading5"]->reference << endl;
  switch(s->type())
  {
    case DocSimpleSect::See: 
      m_t << theTranslator->trSeeAlso(); break;
    case DocSimpleSect::Return: 
      m_t << theTranslator->trReturns(); break;
    case DocSimpleSect::Author: 
      m_t << theTranslator->trAuthor(TRUE,TRUE); break;
    case DocSimpleSect::Authors: 
      m_t << theTranslator->trAuthor(TRUE,FALSE); break;
    case DocSimpleSect::Version: 
      m_t << theTranslator->trVersion(); break;
    case DocSimpleSect::Since: 
      m_t << theTranslator->trSince(); break;
    case DocSimpleSect::Date: 
      m_t << theTranslator->trDate(); break;
    case DocSimpleSect::Note: 
      m_t << theTranslator->trNote(); break;
    case DocSimpleSect::Warning:
      m_t << theTranslator->trWarning(); break;
    case DocSimpleSect::Pre:
      m_t << theTranslator->trPrecondition(); break;
    case DocSimpleSect::Post:
      m_t << theTranslator->trPostcondition(); break;
    case DocSimpleSect::Invar:
      m_t << theTranslator->trInvariant(); break;
    case DocSimpleSect::Remark:
      m_t << theTranslator->trRemarks(); break;
    case DocSimpleSect::Attention:
      m_t << theTranslator->trAttention(); break;
    case DocSimpleSect::User: break;
    case DocSimpleSect::Rcs: break;
    case DocSimpleSect::Unknown:  break;
  }

  // special case 1: user defined title
  if (s->type()!=DocSimpleSect::User && s->type()!=DocSimpleSect::Rcs)
  {
    m_t << ":";
    m_t << "\\par" << endl;
    m_t << "}"; // end bold
    incIndentLevel();
    m_t << rtf_Style_Reset << getStyle("DescContinue");
  }
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocSimpleSect *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocSimpleSect)}\n");
  if (!m_lastIsPara) m_t << "\\par" << endl;
  decIndentLevel();
  m_t << "}"; // end desc
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocTitle *)
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocTitle)}\n");
}

void RTFDocVisitor::visitPost(DocTitle *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocTitle)}\n");
  m_t << "}"; // end bold
  m_t << "\\par" << endl;
  incIndentLevel();
  m_t << rtf_Style_Reset << getStyle("DescContinue");
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPre(DocSimpleList *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocSimpleSect)}\n");
  m_t << "{" << endl;
  rtf_listItemInfo[m_indentLevel].isEnum = FALSE;
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocSimpleList *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocSimpleSect)}\n");
  if (!m_lastIsPara) m_t << "\\par" << endl;
  m_t << "}" << endl;
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocSimpleListItem *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocSimpleListItem)}\n");
  m_t << "\\par" << rtf_Style_Reset << getStyle("ListBullet") << endl;
  m_lastIsPara=FALSE;
  incIndentLevel();
}

void RTFDocVisitor::visitPost(DocSimpleListItem *) 
{
  decIndentLevel();
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocSimpleListItem)}\n");
}

void RTFDocVisitor::visitPre(DocSection *s)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocSection)}\n");
  if (!m_lastIsPara) m_t << "\\par" << endl;
  m_t << "{{" // start section
      << rtf_Style_Reset;
  QString heading;
  int level = QMIN(s->level()+1,4);
  heading.sprintf("Heading%d",level);
  // set style
  m_t << rtf_Style[heading]->reference << endl;
  // make table of contents entry
  filter(s->title());
  m_t << endl << "\\par" << "}" << endl;
  m_t << "{\\tc\\tcl" << level << " \\v ";
  filter(s->title());
  m_t << "}" << endl;
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPost(DocSection *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocSection)}\n");
  m_t << "}" << endl; // end section
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocHtmlList *l)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocHtmlList)}\n");
  m_t << "{" << endl;
  rtf_listItemInfo[m_indentLevel].isEnum = l->type()==DocHtmlList::Ordered; 
  rtf_listItemInfo[m_indentLevel].number = 1;
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocHtmlList *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocHtmlList)}\n");
  m_t << "\\par" << endl << "}" << endl;
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocHtmlListItem *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocHtmlListItem)}\n");
  m_t << "\\par" << endl;
  m_t << rtf_Style_Reset;
  if (rtf_listItemInfo[m_indentLevel].isEnum)
  {
    m_t << getStyle("ListEnum") << endl;
    m_t << rtf_listItemInfo[m_indentLevel].number << ".\\tab ";
    rtf_listItemInfo[m_indentLevel].number++;
  }
  else
  {
    m_t << getStyle("ListBullet") << endl;
  }
  incIndentLevel();
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocHtmlListItem *) 
{
  decIndentLevel();
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocHtmlListItem)}\n");
}

void RTFDocVisitor::visitPre(DocHtmlDescList *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocHtmlDescList)}\n");
  //m_t << "{" << endl;
  //m_t << rtf_Style_Reset << getStyle("ListContinue");
  //m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocHtmlDescList *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocHtmlDescList)}\n");
  //m_t << "}" << endl;
  //m_t << "\\par" << endl;
  //m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocHtmlDescTitle *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocHtmlDescTitle)}\n");
  //m_t << "\\par" << endl;
  //m_t << "{\\b ";
  m_t << "{" << rtf_Style["Heading5"]->reference << endl;
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocHtmlDescTitle *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocHtmlDescTitle)}\n");
  m_t << "\\par" << endl;
  m_t << "}" << endl;
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocHtmlDescData *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocHtmlDescData)}\n");
  incIndentLevel();
  m_t << "{" << rtf_Style_Reset << getStyle("DescContinue");
}

void RTFDocVisitor::visitPost(DocHtmlDescData *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocHtmlDescData)}\n");
  m_t << "\\par" << endl;
  m_t << "}" << endl;
  decIndentLevel();
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocHtmlTable *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocHtmlTable)}\n");
  if (!m_lastIsPara) m_t << "\\par" << endl;
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPost(DocHtmlTable *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocHtmlTable)}\n");
  m_t << "\\pard" << endl;
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocHtmlCaption *)
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocHtmlCaption)}\n");
}

void RTFDocVisitor::visitPost(DocHtmlCaption *) 
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocHtmlCaption)}\n");
}

void RTFDocVisitor::visitPre(DocHtmlRow *r)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocHtmlRow)}\n");
  uint i,columnWidth=rtf_pageWidth/r->numCells();
  m_t << "\\trowd \\trgaph108\\trleft-108"
         "\\trbrdrt\\brdrs\\brdrw10 "
         "\\trbrdrl\\brdrs\\brdrw10 "
         "\\trbrdrb\\brdrs\\brdrw10 "
         "\\trbrdrr\\brdrs\\brdrw10 "
         "\\trbrdrh\\brdrs\\brdrw10 "
         "\\trbrdrv\\brdrs\\brdrw10 "<< endl;
  for (i=0;i<r->numCells();i++)
  {
    m_t << "\\clvertalt\\clbrdrt\\brdrs\\brdrw10 "
           "\\clbrdrl\\brdrs\\brdrw10 "
           "\\clbrdrb\\brdrs\\brdrw10 "
           "\\clbrdrr \\brdrs\\brdrw10 "
           "\\cltxlrtb "
           "\\cellx" << ((i+1)*columnWidth) << endl;
  }
  m_t << "\\pard \\widctlpar\\intbl\\adjustright" << endl;
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocHtmlRow *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocHtmlRow)}\n");
  m_t << endl;
  m_t << "\\pard \\widctlpar\\intbl\\adjustright" << endl;
  m_t << "{\\row }" << endl;
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPre(DocHtmlCell *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocHtmlCell)}\n");
  m_t << "{";
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocHtmlCell *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocHtmlCell)}\n");
  m_t << "\\cell }";
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPre(DocInternal *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocInternal)}\n");
  m_t << "{"; // start desc
  m_t << "{\\b "; // start bold
  m_t << theTranslator->trForInternalUseOnly();
  m_t << "}"; // end bold
  m_t << "\\par" << endl;
  incIndentLevel();
  m_t << rtf_Style_Reset << getStyle("DescContinue");
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocInternal *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocInternal)}\n");
  m_t << "\\par" << endl;
  decIndentLevel();
  m_t << "}"; // end desc
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocHRef *href)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocHRef)}\n");
  if (Config_getBool("RTF_HYPERLINKS"))
  {
    m_t << "{\\field "
             "{\\*\\fldinst "
               "{ HYPERLINK  \\\\l \"" << href->url() << "\" "
               "}{}"
             "}"
             "{\\fldrslt "
               "{\\cs37\\ul\\cf2 ";

  }
  else
  {
    m_t << "{\\f2 ";
  }
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocHRef *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocHRef)}\n");
  if (Config_getBool("RTF_HYPERLINKS"))
  { 
    m_t <<     "}"
             "}"
           "}";
  }
  else
  {
    m_t << "}";
  }
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPre(DocHtmlHeader *header)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocHtmlHeader)}\n");
  m_t << "{" // start section
      << rtf_Style_Reset;
  QString heading;
  int level = QMIN(header->level()+2,4);
  heading.sprintf("Heading%d",level);
  // set style
  m_t << rtf_Style[heading]->reference;
  // make table of contents entry
  m_t << "{\\tc\\tcl \\v " << level << "}";
  m_lastIsPara=FALSE;
  
}

void RTFDocVisitor::visitPost(DocHtmlHeader *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocHtmlHeader)}\n");
  m_t << "\\par" << endl;
  m_t << "}" << endl; // end section
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocImage *img)
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocImage)}\n");
  if (img->type()==DocImage::Rtf)
  {
    m_t << "\\par" << endl;
    m_t << "{" << endl;
    m_t << rtf_Style_Reset << endl;
    m_t << "\\par\\pard \\qc {\\field\\flddirty {\\*\\fldinst INCLUDEPICTURE \"";
    m_t << img->name();
    m_t << "\" \\\\d \\\\*MERGEFORMAT}{\\fldrslt IMAGE}}\\par" << endl;
    m_t << "}" << endl;
    m_lastIsPara=TRUE;
  }
  else // other format -> skip
  {
  }
  // hide caption since it is not supported at the moment
  pushEnabled();
  m_hide=TRUE;
}

void RTFDocVisitor::visitPost(DocImage *) 
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocImage)}\n");
  popEnabled();
}

void RTFDocVisitor::visitPre(DocDotFile *df)
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocDotFile)}\n");
  writeDotFile(df->file());

  // hide caption since it is not supported at the moment
  pushEnabled();
  m_hide=TRUE;
}

void RTFDocVisitor::visitPost(DocDotFile *) 
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocDotFile)}\n");
  popEnabled();
}

void RTFDocVisitor::visitPre(DocLink *lnk)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocLink)}\n");
  startLink(lnk->ref(),lnk->file(),lnk->anchor());
}

void RTFDocVisitor::visitPost(DocLink *lnk) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocLink)}\n");
  endLink(lnk->ref());
}

void RTFDocVisitor::visitPre(DocRef *ref)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocRef)}\n");
  if (!ref->file().isEmpty()) startLink(ref->ref(),ref->file(),ref->anchor());
  if (!ref->hasLinkText()) filter(ref->targetTitle());
}

void RTFDocVisitor::visitPost(DocRef *ref) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocRef)}\n");
  if (!ref->file().isEmpty()) endLink(ref->ref());
  m_t << " ";
}


void RTFDocVisitor::visitPre(DocSecRefItem *)
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocSecRefItem)}\n");
}

void RTFDocVisitor::visitPost(DocSecRefItem *) 
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocSecRefItem)}\n");
}

void RTFDocVisitor::visitPre(DocSecRefList *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocSecRefList)}\n");
  m_t << "{" << endl;
  incIndentLevel();
  m_t << rtf_Style_Reset << getStyle("LatexTOC") << endl;
  m_t << "\\par" << endl;
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPost(DocSecRefList *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocSecRefList)}\n");
  decIndentLevel();
  m_t << "\\par" << endl;
  m_t << "}";
  m_lastIsPara=TRUE;
}

//void RTFDocVisitor::visitPre(DocLanguage *l)
//{
//  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocLanguage)}\n");
//  QString langId = Config_getEnum("OUTPUT_LANGUAGE");
//  if (l->id().lower()!=langId.lower())
//  {
//    pushEnabled();
//    m_hide = TRUE;
//  }
//}
//
//void RTFDocVisitor::visitPost(DocLanguage *l) 
//{
//  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocLanguage)}\n");
//  QString langId = Config_getEnum("OUTPUT_LANGUAGE");
//  if (l->id().lower()!=langId.lower())
//  {
//    popEnabled();
//  }
//}

void RTFDocVisitor::visitPre(DocParamSect *s)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocParamSect)}\n");
  m_t << "{"; // start param list
  if (!m_lastIsPara) m_t << "\\par" << endl;
  //m_t << "{\\b "; // start bold
  m_t << "{" << rtf_Style["Heading5"]->reference << endl;
  switch(s->type())
  {
    case DocParamSect::Param: 
      m_t << theTranslator->trParameters(); break;
    case DocParamSect::RetVal: 
      m_t << theTranslator->trReturnValues(); break;
    case DocParamSect::Exception: 
      m_t << theTranslator->trExceptions(); break;
    default:
      ASSERT(0);
  }
  m_t << ":";
  m_t << "\\par" << endl;
  m_t << "}" << endl;
  incIndentLevel();
  m_t << rtf_Style_Reset << getStyle("DescContinue");
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPost(DocParamSect *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocParamSect)}\n");
  //m_t << "\\par" << endl;
  decIndentLevel();
  m_t << "}" << endl;
}

void RTFDocVisitor::visitPre(DocParamList *pl)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocParamList)}\n");
  m_t << "{\\i ";
  //QStrListIterator li(pl->parameters());
  //const char *s;
  QListIterator<DocNode> li(pl->parameters());
  DocNode *param;
  bool first=TRUE;
  for (li.toFirst();(param=li.current());++li)
  {
    if (!first) m_t << ","; else first=FALSE;
    if (param->kind()==DocNode::Kind_Word)
    {
      visit((DocWord*)param); 
    }
    else if (param->kind()==DocNode::Kind_LinkedWord)
    {
      visit((DocLinkedWord*)param); 
    }
  }
  m_t << "} ";
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPost(DocParamList *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocParamList)}\n");
  m_t << "\\par" << endl;
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocXRefItem *x)
{
  if (m_hide) return;
  bool anonymousEnum = x->file()=="@";
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocXRefItem)}\n");
  m_t << "{"; // start param list
  //m_t << "{\\b "; // start bold
  m_t << "{" << rtf_Style["Heading5"]->reference << endl;
  if (Config_getBool("RTF_HYPERLINKS") && !anonymousEnum)
  {
    QString refName;
    if (!x->file().isEmpty())
    {
      refName+=x->file();
    }
    if (!x->file().isEmpty() && !x->anchor().isEmpty())
    {
      refName+="_";
    }
    if (!x->anchor().isEmpty())
    {
      refName+=x->anchor();
    }

    m_t << "{\\field "
             "{\\*\\fldinst "
               "{ HYPERLINK  \\\\l \"" << refName << "\" "
               "}{}"
             "}"
             "{\\fldrslt "
               "{\\cs37\\ul\\cf2 ";
    filter(x->title());
    m_t <<     "}"
             "}"
           "}";
  }
  else
  {
    filter(x->title());
  }
  m_t << ":";
  m_t << "\\par" << endl;
  m_t << "}"; // end bold
  incIndentLevel();
  m_t << rtf_Style_Reset << getStyle("DescContinue");
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::visitPost(DocXRefItem *)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocXRefItem)}\n");
  m_t << "\\par" << endl;
  decIndentLevel();
  m_t << "}" << endl; // end xref item
  m_lastIsPara=TRUE;
}

void RTFDocVisitor::visitPre(DocInternalRef *ref)
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocInternalRef)}\n");
  startLink("",ref->file(),ref->anchor());
}

void RTFDocVisitor::visitPost(DocInternalRef *) 
{
  if (m_hide) return;
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocInternalRef)}\n");
  endLink("");
  m_t << " ";
}

void RTFDocVisitor::visitPre(DocCopy *)
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocCopy)}\n");
}

void RTFDocVisitor::visitPost(DocCopy *)
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocCopy)}\n");
}

void RTFDocVisitor::visitPre(DocText *)
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPre(DocText)}\n");
}

void RTFDocVisitor::visitPost(DocText *)
{
  DBG_RTF("{\\comment RTFDocVisitor::visitPost(DocText)}\n");
}

static char* getMultiByte(int c)
{
    static char s[10];
    sprintf(s,"\\'%X",c);
    return s;
}

void RTFDocVisitor::filter(const char *str,bool verbatim)
{ 
  if (str)
  {
    const unsigned char *p=(const unsigned char *)str;
    unsigned char c;
    unsigned char pc='\0';
    while (*p)
    {
      static bool MultiByte = FALSE;
      c=*p++;

      if ( MultiByte )
      {
        m_t << getMultiByte( c );
        MultiByte = FALSE;
        continue;
      }
      if ( c >= 0x80 )
      {
        MultiByte = TRUE;
        m_t << getMultiByte( c );
        continue;
      }

      switch (c)
      {
        case '{':  m_t << "\\{";            break;
        case '}':  m_t << "\\}";            break;
        case '\\': m_t << "\\\\";           break;
        case '\n': if (verbatim)
                   {
                     m_t << "\\par" << endl; 
                   }
                   else
                   {
                     m_t << '\n';
                   }
                   break;
        default:   m_t << (char)c;
      }
      pc = c;
    }
  }
}

void RTFDocVisitor::startLink(const QString &ref,const QString &file,const QString &anchor)
{
  if (ref.isEmpty() && Config_getBool("RTF_HYPERLINKS"))
  {
    QString refName;
    if (!file.isEmpty())
    {
      refName+=file;
    }
    if (anchor)
    {
      refName+='_';
      refName+=anchor;
    }

    m_t << "{\\field {\\*\\fldinst { HYPERLINK  \\\\l \"";
    m_t << rtfFormatBmkStr(refName);
    m_t << "\" }{}";
    m_t << "}{\\fldrslt {\\cs37\\ul\\cf2 ";
  }
  else
  {
    m_t << "{\\b ";
  }
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::endLink(const QString &ref)
{
  if (ref.isEmpty() && Config_getBool("RTF_HYPERLINKS"))
  {
    m_t << "}}}";
  }
  else
  {
    m_t << "}";
  }
  m_lastIsPara=FALSE;
}

void RTFDocVisitor::pushEnabled()
{
  m_enabled.push(new bool(m_hide));
}

void RTFDocVisitor::popEnabled()
{
  bool *v=m_enabled.pop();
  ASSERT(v!=0);
  m_hide = *v;
  delete v;
}

void RTFDocVisitor::writeDotFile(const QString &fileName)
{
  QString baseName=fileName;
  int i;
  if ((i=baseName.findRev('/'))!=-1)
  {
    baseName=baseName.right(baseName.length()-i-1);
  } 
  QString outDir = Config_getString("RTF_OUTPUT");
  writeDotGraphFromFile(fileName,outDir,baseName,BITMAP);
  if (!m_lastIsPara) m_t << "\\par" << endl;
  m_t << "{" << endl;
  m_t << rtf_Style_Reset;
  m_t << "\\pard \\qc {\\field\\flddirty {\\*\\fldinst INCLUDEPICTURE \"";
  m_t << baseName << "." << Config_getEnum("DOT_IMAGE_FORMAT");
  m_t << "\" \\\\d \\\\*MERGEFORMAT}{\\fldrslt IMAGE}}\\par" << endl;
  m_t << "}" << endl;
  m_lastIsPara=TRUE;
}

