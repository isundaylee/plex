/*
 *      Copyright (C) 2005-2010 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "VTPDirectory.h"
#include "VTPSession.h"
#include "VideoInfoTag.h"
#include "URL.h"
#include "Util.h"
#include "FileItem.h"


using namespace std;
using namespace XFILE;

CVTPDirectory::CVTPDirectory()
{
  m_session = new CVTPSession();
}

CVTPDirectory::~CVTPDirectory()
{
  delete m_session;
}

bool CVTPDirectory::GetChannels(const CStdString& base, CFileItemList &items)
{
  vector<CVTPSession::Channel> channels;
  if(!m_session->GetChannels(channels))
    return false;

  vector<CVTPSession::Channel>::iterator it;
  for(it = channels.begin(); it != channels.end(); it++)
  {
    CStdString buffer;
    CFileItemPtr item(new CFileItem("", false));

    item->m_strPath.Format("%s/%d.ts", base.c_str(), it->index);
    item->m_strTitle = it->name;
    buffer.Format("%d - %s", it->index, it->name);
    item->SetLabel(buffer);

    items.Add(item);
  }
  return true;
}

bool CVTPDirectory::GetDirectory(const CStdString& strPath, CFileItemList &items)
{
  CURL url(strPath);

  if(url.GetHostName() == "")
    url.SetHostName("localhost");

  CStdString base = url.Get();
  CUtil::RemoveSlashAtEnd(base);

  // add port after, it changes the structure
  // and breaks CUtil::GetMatchingSource
  if(url.GetPort() == 0)
    url.SetPort(2004);

  if(!m_session->Open(url.GetHostName(), url.GetPort()))
    return false;

  if(url.GetFileName().IsEmpty())
  {
    CFileItemPtr item;

    item.reset(new CFileItem(base + "/channels/", true));
    item->SetLabel("Live Channels");
    item->SetLabelPreformated(true);
    items.Add(item);
    return true;
  }
  else if(url.GetFileName() == "channels/")
    return GetChannels(base, items);
  else
    return false;
}

