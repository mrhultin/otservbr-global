/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef FS_NPC_H_9F5EEFE64314418CA7DA41D1B9409DD0
#define FS_NPC_H_9F5EEFE64314418CA7DA41D1B9409DD0

#include "../../items/tile.h"
#include "npcs.h"
#include "../../utils/enums.h"

class Creature;
class Game;
class SpawnNpc;

class Npc final : public Creature
{
	public:
		static Npc* createNpc(const std::string& name);
		static int32_t despawnRange;
		static int32_t despawnRadius;

		explicit Npc(NpcType* npcType);
		~Npc();

		// non-copyable
		Npc(const Npc&) = delete;
		Npc& operator=(const Npc&) = delete;

		Npc* getNpc() override {
			return this;
		}
		const Npc* getNpc() const override {
			return this;
		}

		void setID() override {
			if (id == 0) {
				id = npcAutoID++;
			}
		}

		void removeList() override;
		void addList() override;

		const std::string& getName() const override {
			return npcType->name;
		}
		const std::string& getNameDescription() const override {
			return npcType->nameDescription;
		}
		std::string getDescription(int32_t) const override {
			return strDescription + '.';
		}

		CreatureType_t getType() const override {
			return CREATURETYPE_NPC;
		}

		const Position& getMasterPos() const {
			return masterPos;
		}
		void setMasterPos(Position pos) {
			masterPos = pos;
		}

		void onPlayerEndTrade(Player* player, int32_t shop);
		void onPlayerCloseChannel(Player* player);

		uint8_t getSpeechBubble() const override {
			return npcType->info.speechBubble;
		}
		void setSpeechBubble(const uint8_t bubble) {
			npcType->info.speechBubble = bubble;
		}

		uint16_t getCurrencyTrading() const {
			return npcType->info.currencyClientId;
		}

		uint16_t getCurrency() const {
			return npcType->info.currencyServerId;
		}

		int32_t getArmor() const override {
			return npcType->info.armor;
		}
		int32_t getDefense() const override {
			return npcType->info.defense;
		}

		bool isPushable() const override {
			return false;
		}
		bool isAttackable() const override {
			return false;
		}

		bool canSee(const Position& pos) const override;
		bool canSeeInvisibility() const override {
			return true;
		}
		RespawnType getRespawnType() const {
			return npcType->info.respawnType;
		}
		void setSpawnNpc(SpawnNpc* newSpawn) {
			this->spawnNpc = newSpawn;
		}

		void setPlayerInteraction(uint32_t playerId, uint16_t topicId = 0);
		void updatePlayerInteractions(Player* player);
		void removePlayerInteraction(uint32_t playerId);
		void resetPlayerInteractions();

		bool isInteractingWithPlayer(uint32_t playerId) {
		  if (playerInteractions.find(playerId) == playerInteractions.end()) {
		    return false;
		  }
			return true;
		}

		bool isPlayerInteractingOnTopic(uint32_t playerId, uint16_t topicId) {
			auto it = playerInteractions.find(playerId);
			if (it == playerInteractions.end()) {
				return false;
			}
			return it->second == topicId;
		}

		void onCreatureAppear(Creature* creature, bool isLogin) override;
		void onRemoveCreature(Creature* creature, bool isLogout) override;
		void onCreatureMove(Creature* creature, const Tile* newTile, const Position& newPos, const Tile* oldTile, const Position& oldPos, bool teleport) override;
		void onCreatureSay(Creature* creature, SpeakClasses type, const std::string& text) override;
		void onThink(uint32_t interval) override;
		void onPlacedCreature() override;

		bool canWalkTo(const Position& fromPos, Direction dir) const;
		bool getNextStep(Direction& direction, uint32_t& flags) override;

		void setNormalCreatureLight() override {
			internalLight = npcType->info.light;
		}

		static uint32_t npcAutoID;

	private:
		void addShopPlayer(Player* player);
		void removeShopPlayer(Player* player);
		void closeAllShopWindows();
		void onThinkYell(uint32_t interval);
		void onThinkWalk(uint32_t interval);

		bool isInSpawnRange(const Position& pos) const;

		std::string strDescription;

		std::map<uint32_t, uint16_t> playerInteractions;

		std::set<Player*> shopPlayerSet;

		NpcType* npcType;
		SpawnNpc* spawnNpc = nullptr;

		int32_t masterRadius;

		uint8_t speechBubble;

		uint16_t currencyServerId;
		uint16_t currencyClientId;

		uint32_t yellTicks = 0;
		uint32_t walkTicks = 0;

		bool ignoreHeight;

		Position masterPos;

		friend class LuaScriptInterface;
		friend class Map;
};

#endif
