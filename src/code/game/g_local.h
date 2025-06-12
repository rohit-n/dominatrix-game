//-----------------------------------------------------------------------------
//
//  $Logfile:: /Quake 2 Engine/Sin/code/game/g_local.h                        $
// $Revision:: 100                                                            $
//   $Author:: Aldie                                                          $
//     $Date:: 3/12/99 8:12p                                                  $
//
// Copyright (C) 2020 by Night Dive Studios, Inc.
// All rights reserved.
//
// See the license.txt file for conditions and terms of use for this code.
//
// DESCRIPTION:
// local definitions for game module
//

#ifndef __G_LOCAL_H__
#define __G_LOCAL_H__

#ifndef _WIN32
#define strcmpi(s1, s2) strcasecmp(s1, s2)
#define strnicmp(s1, s2, n) strncasecmp(s1, s2, n)
#endif

#ifndef REF_HARD_LINKED
#if defined(__linux__) || defined(__SWITCH__)
#define EXPORT_FROM_DLL __attribute__ ((visibility("default")))
#else
#define EXPORT_FROM_DLL
#endif
//#define EXPORT_TEMPLATE
#else
#define EXPORT_FROM_DLL
#endif

#include "q_shared.h"

// define GAME_INCLUDE so that game.h does not define the
// short, server-visible gclient_t and edict_t structures,
// because we define the full size ones in this file
#define GAME_INCLUDE

#include "game.h"
#include "container.h"
#include "str.h"

// the "gameversion" client command will print this plus compile date
#define GAMEVERSION "base"

// memory tags to allow dynamic memory to be cleaned up
#define TAG_GAME  765 // clear when unloading the dll
#define TAG_LEVEL 766 // clear when loading a new level

// protocol bytes that can be directly added to messages
#define  svc_muzzleflash     1
#define  svc_muzzleflash2    2
#define  svc_temp_entity     3
#define  svc_layout          4
#define  svc_inventory       5
#define  svc_console_command 6
#define  svc_stufftext       12

// handedness values
#define RIGHT_HANDED  0
#define LEFT_HANDED   1
#define CENTER_HANDED 2

#define random()  ((rand () & 0x7fff) / ((float)0x7fff))
#define crandom() (2.0 * (random() - 0.5))

// predefine Entity so that we can add it to edict_t without any errors
class Entity;

//
// Exported templated classes must be explicitly instantiated
//
#ifdef EXPORT_TEMPLATE
template class EXPORT_FROM_DLL Container<str>;
#endif

// client data that stays across multiple level loads
typedef struct
{
   char        userinfo[MAX_INFO_STRING];
   char        netname[16];
   char        model[MAX_QPATH];
   char        skin[MAX_QPATH];
   int         hand;

   // values saved and restored from edicts when changing levels
   int         health;
   int         max_health;

} client_persistant_t;

// client data that stays across deathmatch respawns
typedef struct
{
   int         enterframe;             // level.framenum the client entered the game
   int         score;                  // frags, etc
   vec3_t      cmd_angles;             // angles sent over in the last command
   int         ctf_team;               // CTF team number
   int         ctf_state;              // CTF player state
   float       ctf_lasttechmsg;        // CTF Last time a tech message was sent
   float       ctf_lastfraggedcarrier; // Last time a carrier was fragged
   float       ctf_lasthurtcarrier;    // List time carrier was hurt
   float       ctf_lastreturnedflag;   // Last time flag was returned
   float       ctf_idtime;             // Last time a player id was made
} client_respawn_t;

// this structure is cleared on each PutClientInServer(),
// except for 'client->pers'
typedef struct gclient_s
{
   // known to server
   player_state_t       ps;   // communicated by server to clients
   int                  ping;

   // private to game
   client_persistant_t  pers;
   client_respawn_t     resp;
   qboolean             showinfo;
} gclient_t;

struct edict_s
{
   entity_state_t  s;
   gclient_t      *client; // NULL if not a player
                           // the server expects the first part
                           // of gclient_t to be a player_state_t
                           // but the rest of it is opaque

   qboolean        inuse;
   int             linkcount;

   // FIXME: move these fields to a server private sv_entity_t
   link_t          area; // linked to a division node or leaf

   int             num_clusters;                  // if -1, use headnode instead
   int             clusternums[MAX_ENT_CLUSTERS];
   int             headnode;                      // unused if num_clusters != -1
   int             areanum, areanum2;

   //================================

   int             svflags;
   vec3_t          mins, maxs;
   vec3_t          absmin, absmax, size;
   vec3_t          fullmins, fullmaxs;
   float           fullradius;
   vec3_t          centroid;
   solid_t         solid;
   int             clipmask;
   edict_t        *owner;


   // DO NOT MODIFY ANYTHING ABOVE THIS, THE SERVER
   // EXPECTS THE FIELDS IN THAT ORDER!

   //================================
   Entity         *entity;
   float           freetime;  // sv.time when the object was freed
   float           spawntime; // sv.time when the object was spawned

   char            entname[64];

   edict_t        *next;
   edict_t        *prev;
};

#include "vector.h"
#include "LINKLIST.H"
#include "class.h"

//
// this structure is left intact through an entire game
// it should be initialized at dll load time, and read/written to
// the server.ssv file for savegames
//

class EXPORT_FROM_DLL game_locals_t : public Class
{
public:
   CLASS_PROTOTYPE(game_locals_t);

   gclient_t        *clients;  // [maxclients]
   qboolean          autosaved;

   // can't store spawnpoint in level, because
   // it would get overwritten by the savegame restore
   str               spawnpoint;          // needed for coop respawns

   // store latched cvars here that we want to get at often
   int               maxclients;
   int               maxentities;
   int               maxconsoles;
   int               maxsurfaces;

   qboolean          force_entnum;
   int               spawn_entnum;

   // List of valid player models loaded from players global scriptfile
   Container<str>    ValidPlayerModels;

   game_locals_t();
   virtual void      Archive(Archiver &arc)   override;
   virtual void      Unarchive(Archiver &arc) override;
};

//
// this structure is cleared as each map is entered
// it is read/written to the level.sav file for savegames
//
class EXPORT_FROM_DLL level_locals_t : public Class
{
public:
   CLASS_PROTOTYPE(level_locals_t);

   int         framenum;
   float       time;

   str         level_name;             // the descriptive name (Outer Base, etc)
   str         mapname;                // the server name (base1, etc)
   str         nextmap;                // go here when fraglimit is hit

   // used for cinematics
   qboolean    playerfrozen;

   // used to prevent players from continuing failed games
   qboolean    missionfailed;
   float       missionfailedtime;

   // intermission state
   float       intermissiontime; // time the intermission was started
   int         exitintermission;

   edict_t    *next_edict;       // Used to keep track of the next edict to process in G_RunFrame

   int         total_secrets;
   int         found_secrets;

   Entity     *current_entity;   // entity running from G_RunFrame

   // FIXME - remove this later when it is passed in the event.
   trace_t     impact_trace;

   int         body_queue;

   float       earthquake;

   qboolean    clearsavegames;
   qboolean    cinematic;

   qboolean    no_jc;

   // Blending color for water, light volumes,lava
   Vector      water_color;
   Vector      lightvolume_color;
   Vector      lava_color;
   float       water_alpha;
   float       lightvolume_alpha;
   float       lava_alpha;
   qboolean    airclamp;
   qboolean    training;

   level_locals_t();
   virtual void   Archive(Archiver &arc)   override;
   virtual void   Unarchive(Archiver &arc) override;
};

#include "g_main.h"
#include "listener.h"
#include "g_utils.h"
#include "g_spawn.h"
#include "g_phys.h"

#endif

// EOF

