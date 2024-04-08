/***************************************************************************
 *   fheroes2: https://github.com/ihhub/fheroes2                           *
 *   Copyright (C) 2023 - 2024                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "map_object_info.h"
#include "resource.h"

class StreamBase;

namespace Maps::Map_Format
{
    struct ObjectInfo
    {
        uint32_t id{ 0 };

        ObjectGroup group{ ObjectGroup::LANDSCAPE_MOUNTAINS };

        uint32_t index{ 0 };
    };

    struct TileInfo
    {
        uint16_t terrainIndex{ 0 };
        uint8_t terrainFlag{ 0 };

        std::vector<ObjectInfo> objects;
    };

    // This structure should be used for any object that require simple data to be saved into map.
    struct StandardObjectMetadata
    {
        std::array<uint32_t, 3> metadata{ 0 };
    };

    struct CastleMetadata
    {
        // Color, type and whether it is castle or town must come from ObjectInfo to make sure
        // that these values correspond to graphical representation of the castle.

        // If the name is empty a random name is going to be set by the engine.
        std::string customName;

        // Defending monsters that are set in the castle. Type 0 means not set.
        std::array<int32_t, 5> defenderMonsterType{ 0 };
        std::array<int32_t, 5> defenderMonsterCount{ 0 };

        // Whether the captain is being hired in the town / castle.
        bool isCaptainAvailable{ false };

        // A list of built buildings.
        std::vector<int32_t> builtBuildings;

        // A list of buildings that cannot be built.
        std::vector<int32_t> bannedBuildings;

        // Spells that must appear in the Magic Guild.
        std::vector<int32_t> mustHaveSpells;

        // Spells that must NOT appear the Magic Guild.
        std::vector<int32_t> bannedSpells;

        // The number of monsters available to hire in dwellings. A negative value means that no change will be applied.
        std::array<int32_t, 6> availableToHireMonsterCount{ -1 };
    };

    struct HeroMetadata
    {
        // Hero's color and type must come from ObjectInfo to make sure
        // that these values correspond to graphical representation of the hero.

        // If the name is empty a random name is going to be set by the engine.
        std::string customName;

        // Custom portrait. A negative or zero (Heroes::UNKNOWN) value means no customization.
        int32_t customPortrait{ 0 };

        // Custom hero army. Type 0 means not set.
        std::array<int32_t, 5> armyMonsterType{ 0 };
        std::array<int32_t, 5> armyMonsterCount{ 0 };

        // Artifacts with metadata. Type 0 means not set.
        std::array<int32_t, 14> artifact{ 0 };
        std::array<int32_t, 14> artifactMetadata{ 0 };

        // Custom spells available in a Spell Book. If no spells are set then defaults spells will be set for the hero.
        std::vector<int32_t> availableSpells;

        // Patrol state of a hero. It is only applicable for AI heroes.
        bool isOnPatrol{ false };
        uint8_t patrolRadius{ 0 };

        // Secondary skills. Type 0 means not set.
        std::array<int32_t, 8> secondarySkill{ 0 };
        std::array<uint8_t, 8> secondarySkillLevel{ 0 };

        // Mutually exclusive settings: either a hero has a custom level or custom experience.
        // If none of them is set default values are applied.
        int16_t customLevel{ -1 };
        int32_t customExperience{ -1 };

        // Primary Skill bonuses. By default they are set to -1 which means that default values will be applied.
        // These values are applied instead of hero's basic primary skills' values and level-up bonuses.
        int16_t customAttack{ -1 };
        int16_t customDefense{ -1 };
        int16_t customKnowledge{ -1 };
        int16_t customSpellPower{ -1 };

        // The amount of magic points (mana). Negative value means it is not set.
        int16_t magicPoints{ -1 };

        // Hero race.
        uint8_t race{ 0 };

        bool operator==( const HeroMetadata & anotherHeroMetadata ) const
        {
            return customName == anotherHeroMetadata.customName && customPortrait == anotherHeroMetadata.customPortrait && isOnPatrol == anotherHeroMetadata.isOnPatrol
                   && patrolRadius == anotherHeroMetadata.patrolRadius && customLevel == anotherHeroMetadata.customLevel
                   && customExperience == anotherHeroMetadata.customExperience && customAttack == anotherHeroMetadata.customAttack
                   && customDefense == anotherHeroMetadata.customDefense && customKnowledge == anotherHeroMetadata.customKnowledge
                   && customSpellPower == anotherHeroMetadata.customSpellPower && magicPoints == anotherHeroMetadata.magicPoints && race == anotherHeroMetadata.race
                   && armyMonsterType == anotherHeroMetadata.armyMonsterType && armyMonsterCount == anotherHeroMetadata.armyMonsterCount
                   && artifact == anotherHeroMetadata.artifact && artifactMetadata == anotherHeroMetadata.artifactMetadata
                   && availableSpells == anotherHeroMetadata.availableSpells && secondarySkill == anotherHeroMetadata.secondarySkill
                   && secondarySkillLevel == anotherHeroMetadata.secondarySkillLevel;
        }

        bool operator!=( const HeroMetadata & anotherHeroMetadata ) const
        {
            return !( *this == anotherHeroMetadata );
        }
    };

    struct SphinxMetadata
    {
        std::string question;

        std::vector<std::string> answers;

        // An artifact to be given as a reward.
        int32_t artifact{ 0 };

        // Resources to be given as a reward.
        Funds resources;
    };

    struct SignMetadata
    {
        std::string message;
    };

    struct AdventureMapEventMetadata
    {
        std::string message;

        uint8_t humanPlayerColors{ 0 };

        uint8_t computerPlayerColors{ 0 };

        // Does this event occur only once?
        bool isRecurringEvent{ false };

        // An artifact to be given as a reward.
        int32_t artifact{ 0 };

        // Resources to be given as a reward.
        Funds resources;
    };

    struct BaseMapFormat
    {
        // TODO: change it only once the Editor is released to public and there is a need to expand map format functionality.
        uint16_t version{ 1 };
        bool isCampaign{ false };

        uint8_t difficulty{ 0 };

        uint8_t availablePlayerColors{ 0 };
        uint8_t humanPlayerColors{ 0 };
        uint8_t computerPlayerColors{ 0 };
        std::vector<uint8_t> alliances;

        // Only 6 players are allowed per map.
        std::array<uint8_t, 6> playerRace{ 0 };

        uint8_t victoryConditionType{ 0 };
        bool isVictoryConditionApplicableForAI{ false };
        bool allowNormalVictory{ false };
        std::vector<uint32_t> victoryConditionMetadata;

        uint8_t lossCondition{ 0 };
        std::vector<uint32_t> lossConditionMetadata;

        int32_t size{ 0 };

        std::string name;
        std::string description;
    };

    struct MapFormat : public BaseMapFormat
    {
        // This is used only for campaign maps.
        std::vector<uint32_t> additionalInfo;

        std::vector<TileInfo> tiles;

        // These are metadata maps in relation to object UID.
        std::map<uint32_t, StandardObjectMetadata> standardMetadata;

        std::map<uint32_t, CastleMetadata> castleMetadata;

        std::map<uint32_t, HeroMetadata> heroMetadata;

        std::map<uint32_t, SphinxMetadata> sphinxMetadata;

        std::map<uint32_t, SignMetadata> signMetadata;

        std::map<uint32_t, AdventureMapEventMetadata> adventureMapEventMetadata;
    };

    bool loadBaseMap( const std::string & path, BaseMapFormat & map );
    bool loadMap( const std::string & path, MapFormat & map );

    bool saveMap( const std::string & path, const MapFormat & map );

    StreamBase & operator<<( StreamBase & msg, const ObjectInfo & object );
    StreamBase & operator>>( StreamBase & msg, ObjectInfo & object );

    StreamBase & operator<<( StreamBase & msg, const TileInfo & tile );
    StreamBase & operator>>( StreamBase & msg, TileInfo & tile );

    StreamBase & operator<<( StreamBase & msg, const StandardObjectMetadata & metadata );
    StreamBase & operator>>( StreamBase & msg, StandardObjectMetadata & metadata );

    StreamBase & operator<<( StreamBase & msg, const CastleMetadata & metadata );
    StreamBase & operator>>( StreamBase & msg, CastleMetadata & metadata );

    StreamBase & operator<<( StreamBase & msg, const HeroMetadata & metadata );
    StreamBase & operator>>( StreamBase & msg, HeroMetadata & metadata );

    StreamBase & operator<<( StreamBase & msg, const SphinxMetadata & metadata );
    StreamBase & operator>>( StreamBase & msg, SphinxMetadata & metadata );

    StreamBase & operator<<( StreamBase & msg, const SignMetadata & metadata );
    StreamBase & operator>>( StreamBase & msg, SignMetadata & metadata );

    StreamBase & operator<<( StreamBase & msg, const AdventureMapEventMetadata & metadata );
    StreamBase & operator>>( StreamBase & msg, AdventureMapEventMetadata & metadata );

    StreamBase & operator<<( StreamBase & msg, const BaseMapFormat & map );
    StreamBase & operator>>( StreamBase & msg, BaseMapFormat & map );

    StreamBase & operator<<( StreamBase & msg, const MapFormat & map );
    StreamBase & operator>>( StreamBase & msg, MapFormat & map );
}
