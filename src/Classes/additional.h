#ifndef ADDITIONAL_H
#define ADDITIONAL_H

#include <QString>

enum SenderFunc {
    addUrlFile,
    addLocalFile
};

struct M3UFormat
{
//    M3UFormat() {

//    };
    const QString begin_file {"#EXTM3U"};
    const QString begin_channelInfo {"#EXTINF:"};
    const QString begin_group {"#EXTGRP:"};
    const QString begin_logoPath {"#EXTIMG:"};

    const QString epg_code {"tvg-id="};
    const QString epg_name {"tvg-name="};
    const QString logo_path[2] {"tvg-logo=", "logo="};
    const QString logo_path_small {"tvg-logo-small="};
    const QString archive_days[2] {"tvg-rec=", "timeshift="};
    const QString group_name {"group-title="};
    const QString epg_language {"tvg-language="};
    const QString epg_country {"tvg-country="};
    const QString epg_url {"tvg-url="};

    const QString channel_url {"http"};
};

#endif // ADDITIONAL_H
