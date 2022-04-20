#include "playlistparsethread.h"

#include <QFile>

PlaylistParseThread::PlaylistParseThread(QObject *parent)
    : QThread{parent}
{

}

void PlaylistParseThread::run()
{
    m_isRunning = true;
    parseFile();
}

void PlaylistParseThread::prepareParsing(const QString &filePath, const int &idPlaylist, const int &idFile)
{
    m_filePath = filePath;
    m_idPlaylist = idPlaylist;
    m_idFile = idFile;
}

bool PlaylistParseThread::isRunning() const
{
    return m_isRunning;
}

void PlaylistParseThread::pushOutCurChannel()
{
    if (m_channel.idChannel() == -1) {
        m_channels.push(m_channel);
        m_channel.clear();
    }
    if (!m_channels.empty() && m_outChannel.idChannel() == 0/* && !m_isPausing*/) {
        m_outChannel = m_channels.front();
        qDebug() << "PlaylistParseThread: parseLine: end: " << m_outChannel.naim() << " arh: " << m_outChannel.archDays() << " url: " << m_outChannel.url();
        emit channelParsed(m_outChannel, m_idPlaylist, m_idFile, m_outChannel.naim());
    }
}

void PlaylistParseThread::setPauseParsing(const bool &isPausing)
{
    m_mutex.lock();
    if (m_isPausing != isPausing) {
        qDebug() << "PlaylistParseThread: pausing: " << isPausing;
        m_isPausing = isPausing;
    }
    m_mutex.unlock();
}

void PlaylistParseThread::deleteFrontChannel()
{
    qDebug() << "PlaylistParseThread::deleteFrontChannel(): ";
    if (m_outChannel.idChannel() != 0)
        m_outChannel.clear();
    if (!m_channels.empty()) {
        m_channels.pop();
    }
    if (m_channels.empty() && !m_isRunning && m_channel.idChannel() == 0)
        emit fullyParsed(m_idPlaylist, m_idFile);
    else
        pushOutCurChannel();
}

void PlaylistParseThread::parseFile()
{
    qDebug() << "PlaylistParseThread: parseFile: " << m_idPlaylist << " IdFile: " << m_idFile;
    QFile file(m_filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream fileStream(&file);
        QString line = fileStream.readLine();
        if (line.contains(m_format.begin_file, Qt::CaseInsensitive)) {            
            m_channel.clear();
            while (!fileStream.atEnd()) {
                line = fileStream.readLine();
                if (!line.isEmpty()) {
                    parseLine(line);
                }
                if (!m_isRunning)
                    break;
            }
        }
    }
    m_isRunning = false;
    m_isPausing = false;
}

void PlaylistParseThread::parseLine(QString &line)
{
    if (line.startsWith(m_format.begin_channelInfo, Qt::CaseInsensitive)) {
        pushOutCurChannel();
        m_channel.clear();
        idChannel = -1;
        m_channel.setIdChannel(idChannel);
        m_channel.setIdFile(m_idFile);
        m_channel.setIdPlaylist(m_idPlaylist);

        line.remove(m_format.begin_channelInfo);
        removeLineBegin(line);
        checkForSpecSymbols(line);
        parseChannelName(line);
    } else {
        checkForSpecSymbols(line);
        parseChannelName(line);
        if (line.startsWith(m_format.begin_group, Qt::CaseInsensitive)) {
            line.remove(m_format.begin_group);
            QString groupName = line;
            int idCat;
            if (!m_catGW.find(0, groupName))
                m_catGW.insert(0, groupName, true);
            idCat = m_catGW.findIdCategory(groupName);
            m_channel.setIdCategory(idCat);            
        }
        if (line.startsWith(m_format.begin_logoPath, Qt::CaseInsensitive)) {
            line.remove(m_format.begin_logoPath);
            //TODO: try to find logo url in logos and save it to Logos table, after get id_logo for m_channel
        }
        if (line.startsWith(m_format.channel_url, Qt::CaseInsensitive)) {
            m_channel.setUrl(line);
        }
    }
}

void PlaylistParseThread::parseChannelName(QString &line)
{
    if (line.length() > 0) {
        QString naim {};
        int i {};
        for (i = 0; i < line.length() - 1; ++i) {
            if (line[i] == ',')
                break;
        }
        if (i < line.length() - 1) {
            naim = line.sliced(i + 1);
            naim = naim.trimmed();
            line.remove(i, line.length() - i);
            m_channel.setNaim(naim);
        }
    }
}

void PlaylistParseThread::checkForSpecSymbols(QString &line)
{
    if (line.length() > 0) {
        QString value {};
//        if (line.contains(m_format.epg_code, Qt::CaseInsensitive))
//            return true;
//        if (line.contains(m_format.epg_name, Qt::CaseInsensitive))
//            return true;
//        if (line.contains(m_format.logo_path[0], Qt::CaseInsensitive)) {
//            value = getSpecValue(line, m_format.logo_path[0]);
//            if (value.contains("http")) {
////                m_channel.set
//            }
//        }
//        if (line.contains(m_format.logo_path[1], Qt::CaseInsensitive)) {
//            value = getSpecValue(line, m_format.logo_path[1]);
//            if (value.contains("http")) {
////                m_channel.set
//            }
//        }
//        if (line.contains(m_format.logo_path_small, Qt::CaseInsensitive)) {
//            value = getSpecValue(line, m_format.logo_path_small);
//            if (value.contains("http")) {
////                m_channel.set
//            }
//        }
        if (line.contains(m_format.archive_days[0], Qt::CaseInsensitive)) {
            value = getSpecValue(line, m_format.archive_days[0]);
            int intValue = value.toInt();
            m_channel.setArchDays(intValue);
        }
        if (line.contains(m_format.archive_days[1], Qt::CaseInsensitive)) {
            value = getSpecValue(line, m_format.archive_days[0]);
            int intValue = value.toInt();
            m_channel.setArchDays(intValue);
        }

//        if (line.contains(m_format.group_name, Qt::CaseInsensitive))
//            return true;
    }
}

QString PlaylistParseThread::getSpecValue(QString &line, const QString &paramName)
{
    int paramLength = paramName.length();
    QString subStr;
    QString upParamName = paramName.toUpper();
    bool isFound {false};
    uint posBegin {};
    uint posEnd {};
    for (int i = 0; i < line.length() - 1; ++i) {
        if (i + paramLength - 1 <= line.length() - 1)
            subStr = line.sliced(i, paramLength).toUpper();
        else
            break;
        if (subStr == upParamName) {    // param was found, now looking for param value
            posBegin = i;
            posEnd = posBegin + paramLength;
            isFound = true;
            subStr.clear();
            for (int j = i + paramLength; j < line.length() - 1; ++j) {
                if (line[j].isSpace() || line[j] == ',') {
                    posEnd = j - 1;
                    break;
                }
                subStr += line[j];
            }
            break;
        }
    }
    if (isFound) {
        line.remove(posBegin, posEnd - posBegin + 1);
        subStr.remove("\"");
    } else {
        subStr.clear();
    }
    return subStr;
}

void PlaylistParseThread::removeLineBegin(QString &line)
{
    while (!line[0].isLetter()
           && (line[0] == '-' || line[0].isDigit() || line[0].isSpace() || line[0] == ' '))
        line.remove(0, 1);
}
