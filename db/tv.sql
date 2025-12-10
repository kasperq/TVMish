--
-- File generated with SQLiteStudio v3.3.3 on Чт мар 3 13:35:46 2022
--
-- Text encoding used: UTF-8
--
PRAGMA foreign_keys = off;
BEGIN TRANSACTION;

-- Table: CATEGORY
CREATE TABLE CATEGORY (
    ID_CATEGORY INTEGER       PRIMARY KEY AUTOINCREMENT
                              NOT NULL,
    NAME        VARCHAR (100) 
);

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         1,
                         'новости'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         2,
                         'кино'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         3,
                         'музыка'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         4,
                         'познавательные'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         5,
                         'детские'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         6,
                         'развлекательные'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         7,
                         'другие'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         8,
                         'спорт'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         9,
                         'HD'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         10,
                         'взрослые'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         11,
                         'українські'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         12,
                         'USA'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         13,
                         'беларускія'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         14,
                         'HD Orig'
                     );

INSERT INTO CATEGORY (
                         ID_CATEGORY,
                         NAME
                     )
                     VALUES (
                         15,
                         '4K'
                     );


-- Table: CHANNELS
CREATE TABLE CHANNELS (
    ID_CHANNEL   INTEGER       PRIMARY KEY AUTOINCREMENT,
    NAIM         VARCHAR (200),
    URL          VARCHAR (500),
    ID_FILE      INTEGER       REFERENCES PLAYLIST_FILE (ID_FILE) ON DELETE CASCADE
                                                                  ON UPDATE CASCADE,
    ID_PLAYLIST  INTEGER       REFERENCES PLAYLIST (ID_PLAYLIST) ON DELETE CASCADE
                                                                 ON UPDATE CASCADE,
    ID_CATEGORY  INTEGER       REFERENCES CATEGORY (ID_CATEGORY) ON DELETE SET DEFAULT
                                                                 ON UPDATE CASCADE
                               DEFAULT (1),
    NUM_FILE     INTEGER,
    NUM_PLAYLIST INTEGER,
    ARCH_DAYS    INTEGER       DEFAULT (0),
    ID_LOGO      INTEGER       REFERENCES LOGOS (ID_LOGO) ON DELETE SET NULL
                                                          ON UPDATE CASCADE
);

INSERT INTO CHANNELS (
                         ID_CHANNEL,
                         NAIM,
                         URL,
                         ID_FILE,
                         ID_PLAYLIST,
                         ID_CATEGORY,
                         NUM_FILE,
                         NUM_PLAYLIST,
                         ARCH_DAYS,
                         ID_LOGO
                     )
                     VALUES (
                         1,
                         'РБК 50',
                         'http://vqwkxbrk.megogo.xyz/iptv/4A7CXCHXVV2U4Z/10029/index.m3u',
                         59,
                         2,
                         1,
                         1,
                         1,
                         0,
                         'NULL'
                     );

INSERT INTO CHANNELS (
                         ID_CHANNEL,
                         NAIM,
                         URL,
                         ID_FILE,
                         ID_PLAYLIST,
                         ID_CATEGORY,
                         NUM_FILE,
                         NUM_PLAYLIST,
                         ARCH_DAYS,
                         ID_LOGO
                     )
                     VALUES (
                         2,
                         'Инфоканал',
                         'http://vqwkxbrk.megogo.xyz/iptv/4A7CXCHXVV2U4Z/435/index.m3u',
                         59,
                         2,
                         1,
                         2,
                         2,
                         0,
                         'NULL'
                     );

INSERT INTO CHANNELS (
                         ID_CHANNEL,
                         NAIM,
                         URL,
                         ID_FILE,
                         ID_PLAYLIST,
                         ID_CATEGORY,
                         NUM_FILE,
                         NUM_PLAYLIST,
                         ARCH_DAYS,
                         ID_LOGO
                     )
                     VALUES (
                         3,
                         'РБК-ТВ',
                         'http://vqwkxbrk.megogo.xyz/iptv/4A7CXCHXVV2U4Z/106/index.m3u',
                         59,
                         2,
                         1,
                         3,
                         3,
                         7,
                         'NULL'
                     );

INSERT INTO CHANNELS (
                         ID_CHANNEL,
                         NAIM,
                         URL,
                         ID_FILE,
                         ID_PLAYLIST,
                         ID_CATEGORY,
                         NUM_FILE,
                         NUM_PLAYLIST,
                         ARCH_DAYS,
                         ID_LOGO
                     )
                     VALUES (
                         4,
                         'FOX News Extra AU',
                         'http://vqwkxbrk.megogo.xyz/iptv/4A7CXCHXVV2U4Z/6116/index.m3u',
                         59,
                         2,
                         1,
                         4,
                         4,
                         0,
                         'NULL'
                     );


-- Table: FORMAT
CREATE TABLE FORMAT (
    ID_FORMAT INTEGER     PRIMARY KEY AUTOINCREMENT,
    NAIM      STRING (10) 
);

INSERT INTO FORMAT (
                       ID_FORMAT,
                       NAIM
                   )
                   VALUES (
                       1,
                       'm3u8'
                   );

INSERT INTO FORMAT (
                       ID_FORMAT,
                       NAIM
                   )
                   VALUES (
                       2,
                       'm3u'
                   );

INSERT INTO FORMAT (
                       ID_FORMAT,
                       NAIM
                   )
                   VALUES (
                       3,
                       'avi'
                   );

INSERT INTO FORMAT (
                       ID_FORMAT,
                       NAIM
                   )
                   VALUES (
                       5,
                       'mkv'
                   );


-- Table: LOGOS
CREATE TABLE LOGOS (
    ID_LOGO         INTEGER        PRIMARY KEY AUTOINCREMENT,
    CHANNEL_NAME    VARCHAR (200),
    EPG_CODE        VARCHAR (50),
    LOGO            BLOB,
    LOGO_SMALL      BLOB,
    LOGO_PATH       VARCHAR (1000),
    LOGO_SMALL_PATH VARCHAR (1000) 
);

INSERT INTO LOGOS (
                      ID_LOGO,
                      CHANNEL_NAME,
                      EPG_CODE,
                      LOGO,
                      LOGO_SMALL,
                      LOGO_PATH,
                      LOGO_SMALL_PATH
                  )
                  VALUES (
                      1,
                      NULL,
                      NULL,
                      NULL,
                      NULL,
                      NULL,
                      NULL
                  );


-- Table: PLAYLIST
CREATE TABLE PLAYLIST (
    ID_PLAYLIST      INTEGER       PRIMARY KEY AUTOINCREMENT,
    NAIM             VARCHAR (200),
    IS_CURRENT       BOOLEAN,
    DATE_TIME_UPDATE DATETIME,
    DATE_TIME_INSERT DATETIME,
    NUM              INTEGER
);

INSERT INTO PLAYLIST (
                         ID_PLAYLIST,
                         NAIM,
                         IS_CURRENT,
                         DATE_TIME_UPDATE,
                         DATE_TIME_INSERT,
                         NUM
                     )
                     VALUES (
                         2,
                         '22',
                         1,
                         NULL,
                         '2022-02-01 08:18:07',
                         1
                     );

INSERT INTO PLAYLIST (
                         ID_PLAYLIST,
                         NAIM,
                         IS_CURRENT,
                         DATE_TIME_UPDATE,
                         DATE_TIME_INSERT,
                         NUM
                     )
                     VALUES (
                         3,
                         '33',
                         0,
                         '2022-02-01 08:18:07',
                         '2022-02-01 08:17:43',
                         2
                     );

INSERT INTO PLAYLIST (
                         ID_PLAYLIST,
                         NAIM,
                         IS_CURRENT,
                         DATE_TIME_UPDATE,
                         DATE_TIME_INSERT,
                         NUM
                     )
                     VALUES (
                         4,
                         '44',
                         0,
                         '2022-02-01 08:18:07',
                         '2021-09-14 16:23:55',
                         3
                     );

INSERT INTO PLAYLIST (
                         ID_PLAYLIST,
                         NAIM,
                         IS_CURRENT,
                         DATE_TIME_UPDATE,
                         DATE_TIME_INSERT,
                         NUM
                     )
                     VALUES (
                         5,
                         '55',
                         0,
                         '2022-02-01 08:18:07',
                         '2021-09-14 16:33:52',
                         4
                     );

INSERT INTO PLAYLIST (
                         ID_PLAYLIST,
                         NAIM,
                         IS_CURRENT,
                         DATE_TIME_UPDATE,
                         DATE_TIME_INSERT,
                         NUM
                     )
                     VALUES (
                         6,
                         '66',
                         0,
                         '2022-02-04 22:11:03',
                         '2021-09-14 16:33:56',
                         5
                     );

INSERT INTO PLAYLIST (
                         ID_PLAYLIST,
                         NAIM,
                         IS_CURRENT,
                         DATE_TIME_UPDATE,
                         DATE_TIME_INSERT,
                         NUM
                     )
                     VALUES (
                         7,
                         '7',
                         0,
                         '2022-02-04 22:11:03',
                         '2022-01-31 17:58:52',
                         6
                     );


-- Table: PLAYLIST_CHANELS
CREATE TABLE PLAYLIST_CHANELS (
    ID_PLAYLIST_CHANEL INTEGER PRIMARY KEY AUTOINCREMENT,
    ID_CHANEL          INTEGER REFERENCES CHANNELS (ID_CHANNEL) ON DELETE CASCADE
                                                                ON UPDATE CASCADE,
    ID_PLAYLIST        INTEGER REFERENCES PLAYLIST (ID_PLAYLIST) ON DELETE CASCADE
                                                                 ON UPDATE CASCADE
);


-- Table: PLAYLIST_FILE
CREATE TABLE PLAYLIST_FILE (
    ID_FILE         INTEGER       PRIMARY KEY AUTOINCREMENT,
    ID_PLAYLIST     INTEGER       REFERENCES PLAYLIST (ID_PLAYLIST) ON DELETE CASCADE
                                                                    ON UPDATE CASCADE,
    FILE_NAME       STRING (2000),
    FILE_PATH       STRING (3000),
    FILE_PATH_LOCAL STRING (3000),
    ID_FORMAT       INTEGER       REFERENCES FORMAT (ID_FORMAT),
    IS_AVAILABLE    BOOLEAN
);

INSERT INTO PLAYLIST_FILE (
                              ID_FILE,
                              ID_PLAYLIST,
                              FILE_NAME,
                              FILE_PATH,
                              FILE_PATH_LOCAL,
                              ID_FORMAT,
                              IS_AVAILABLE
                          )
                          VALUES (
                              1,
                              1,
                              'file1',
                              '',
                              '',
                              2,
                              0
                          );

INSERT INTO PLAYLIST_FILE (
                              ID_FILE,
                              ID_PLAYLIST,
                              FILE_NAME,
                              FILE_PATH,
                              FILE_PATH_LOCAL,
                              ID_FORMAT,
                              IS_AVAILABLE
                          )
                          VALUES (
                              2,
                              1,
                              'file2',
                              '',
                              '',
                              2,
                              0
                          );

INSERT INTO PLAYLIST_FILE (
                              ID_FILE,
                              ID_PLAYLIST,
                              FILE_NAME,
                              FILE_PATH,
                              FILE_PATH_LOCAL,
