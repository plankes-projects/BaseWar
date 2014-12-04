EXECUTABLE = BaseWar

INCLUDES = -I.. -I../Classes

SOURCES = main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Sound/SoundHandler.cpp \
                   ../../Classes/SceneControl.cpp \
                   ../../Classes/Network/GameStat.cpp \
                   ../../Classes/Network/ServerCommunication.cpp \
                   ../../Classes/Network/NetworkSingleton.cpp \
                   ../../Classes/View/Animations/Blood.cpp \
                   ../../Classes/View/Animations/Explosion.cpp \
                   ../../Classes/View/Animations/SmokeBomb.cpp \
                   ../../Classes/View/Animations/BW_Animation.cpp \
                   ../../Classes/View/BW_Sprite.cpp \
                   ../../Classes/View/Healthbar.cpp \
                   ../../Classes/View/CCAlertView.cpp \
                   ../../Classes/View/CCConfirmView.cpp \
                   ../../Classes/View/Layers/GameOverLayer/GameOverLayer.cpp \
                   ../../Classes/View/Layers/NetworkQueueLayer/NetworkQueueLayer.cpp \
                   ../../Classes/View/Layers/StatsLayer/StatsLayer.cpp \
                   ../../Classes/View/ScrollLayer.cpp \
                   ../../Classes/View/Layers/FeedbackLayer/FeedbackLayer.cpp \
                   ../../Classes/View/Layers/FeedbackLayer/FeedbackQuestionLayer.cpp \
                   ../../Classes/View/Layers/InGameLayer/InGamePauseLayer.cpp \
                   ../../Classes/View/Layers/InGameLayer/InGameSpawnInfo.cpp \
                   ../../Classes/View/Layers/InGameLayer/InGameTopMenuLayer.cpp \
                   ../../Classes/View/Layers/InGameLayer/InGameUnitInfo.cpp \
                   ../../Classes/View/Layers/InGameLayer/InGameUnitsInfo.cpp \
                   ../../Classes/View/Layers/InGameLayer/InGameGameLayer.cpp \
                   ../../Classes/View/Layers/InGameLayer/InGameBottomMenuLayer.cpp \
                   ../../Classes/View/Layers/InGameLayer/Tutorial.cpp \
                   ../../Classes/View/Layers/StartMenuLayer/StartMenuLayer.cpp \
                   ../../Classes/View/Layers/LoadingSpriteLayer/LoadingSpriteLayer.cpp \
                   ../../Classes/View/Layers/RaceLayer/RaceLayer.cpp \
                   ../../Classes/View/Layers/AchievementLayer/AchievementLayer.cpp \
                   ../../Classes/Tools/BW_Point.cpp \
                   ../../Classes/Tools/BW_Rect.cpp \
                   ../../Classes/Tools/BW_Size.cpp \
                   ../../Classes/Tools/Trigonometric.cpp \
                   ../../Classes/Tools/OfflineLogger.cpp \
                   ../../Classes/Tools/BW_Time.cpp \
                   ../../Classes/Tools/Tools.cpp \
                   ../../Classes/Tools/ViewTools.cpp \
                   ../../Classes/Tools/GameStatistics.cpp \
                   ../../Classes/Tools/SupportHandler.cpp \
                   ../../Classes/Debug/BalanceSingleton.cpp \
                   ../../Classes/Model/Attacks/Attack.cpp \
                   ../../Classes/Model/Attacks/SuizideAttack.cpp \
                   ../../Classes/Model/Attacks/ProjectileAttack.cpp \
                   ../../Classes/Model/Attacks/InstantAttack.cpp \
                   ../../Classes/Model/Attacks/CleaveAttack.cpp \
                   ../../Classes/Model/Attacks/PoisonCleaveAttack.cpp \
                   ../../Classes/Model/Attacks/InstantHeal.cpp \
                   ../../Classes/Model/Attacks/LifeLeechAttack.cpp \
                   ../../Classes/Model/Attacks/Projectiles/SplashBombProjectile.cpp \
                   ../../Classes/Model/Attacks/Projectiles/FieryBallProjectile.cpp \
                   ../../Classes/Model/Attacks/Projectiles/NormalProjectile.cpp \
                   ../../Classes/Model/Attacks/Projectiles/ArrowProjectile.cpp \
                   ../../Classes/Model/Attacks/Projectiles/FireBallProjectile.cpp \
                   ../../Classes/Model/Attacks/Projectiles/Projectile.cpp \
                   ../../Classes/Model/Attacks/Projectiles/ProjectileFactory.cpp \
                   ../../Classes/Model/Attacks/Projectiles/IceProjectile.cpp \
                   ../../Classes/Model/Armours/Armour.cpp \
                   ../../Classes/Model/Armours/HeavyArmour.cpp \
                   ../../Classes/Model/Armours/UnarmouredArmour.cpp \
                   ../../Classes/Model/Armours/LightArmour.cpp \
                   ../../Classes/Model/Armours/NormalArmour.cpp \
                   ../../Classes/Model/Armours/LifeRegArmour.cpp \
                   ../../Classes/Model/Effects/Effect.cpp \
                   ../../Classes/Model/Effects/UpgradeUnitEffect.cpp \
                   ../../Classes/Model/Effects/AlterHealReceivedEffect.cpp \
                   ../../Classes/Model/Effects/PoisonEffect.cpp \
                   ../../Classes/Model/Effects/RageEffect.cpp \
                   ../../Classes/Model/Effects/DefenceEffect.cpp \
                   ../../Classes/Model/Effects/IceEffect.cpp \
                   ../../Classes/Model/Effects/ImmuneEffect.cpp \
                   ../../Classes/Model/Skills/LastStandSkill.cpp \
                   ../../Classes/Model/Skills/ReduceReceivedHealSkill.cpp \
                   ../../Classes/Model/Skills/Skill.cpp \
                   ../../Classes/Model/Skills/TeleportAttack.cpp \
                   ../../Classes/Model/Skills/TeleportEscape.cpp \
                   ../../Classes/Model/Skills/RageSkill.cpp \
                   ../../Classes/Model/Skills/DespairSkill.cpp \
                   ../../Classes/Model/Skills/ManaBurnSkill.cpp \
                   ../../Classes/Model/Skills/RemoveBuffSkill.cpp \
                   ../../Classes/Model/Skills/RemoveDebuffSkill.cpp \
                   ../../Classes/Model/Units/Slime.cpp \
                   ../../Classes/Model/Units/Defector.cpp \
                   ../../Classes/Model/Units/Soldier.cpp \
                   ../../Classes/Model/Units/Race.cpp \
                   ../../Classes/Model/Units/RaceChaos.cpp \
                   ../../Classes/Model/Units/Golem.cpp \
                   ../../Classes/Model/Units/FlareAntlion.cpp \
                   ../../Classes/Model/Units/FlareTroll.cpp \
                   ../../Classes/Model/Units/FlareSkelet.cpp \
                   ../../Classes/Model/Units/Tank.cpp \
                   ../../Classes/Model/Units/BotanicArcher.cpp \
                   ../../Classes/Model/Units/BotanicDaemon.cpp \
                   ../../Classes/Model/Units/BotanicDog.cpp \
                   ../../Classes/Model/Units/BotanicFemmage.cpp \
                   ../../Classes/Model/Units/BotanicKnight.cpp \
                   ../../Classes/Model/Units/BotanicMage.cpp \
                   ../../Classes/Model/Units/BotanicPriest.cpp \
                   ../../Classes/Model/Units/Sheep.cpp \
                   ../../Classes/Model/Units/Unit.cpp \
                   ../../Classes/Model/Units/UnitFactory.cpp \
                   ../../Classes/Model/Units/NinjaMelee.cpp \
                   ../../Classes/Model/Units/NinjaRange.cpp \
                   ../../Classes/Model/Units/Pinkie.cpp \
                   ../../Classes/Model/Units/Robot.cpp \
                   ../../Classes/Model/Units/Minion.cpp \
                   ../../Classes/Model/Units/Archer.cpp \
                   ../../Classes/Model/Units/Darksaber.cpp \
                   ../../Classes/Model/Units/MoveAbleUnit.cpp \
                   ../../Classes/Model/Units/Base.cpp \
                   ../../Classes/Model/Units/Tower.cpp \
                   ../../Classes/Model/Units/Baron.cpp \
                   ../../Classes/Model/Units/Viking.cpp \
                   ../../Classes/Model/Units/Troll.cpp \
                   ../../Classes/Model/Units/ModernSoldier.cpp \
                   ../../Classes/AchievementSystem/AchievementSystem.cpp \
                   ../../Classes/AchievementSystem/Achievement.cpp \
                   ../../Classes/AchievementSystem/AchievementFeedback.cpp \
                   ../../Classes/AchievementSystem/AchievementWin.cpp \
                   ../../Classes/AchievementSystem/AchievementStartedBaseWar.cpp \
                   ../../Classes/AchievementSystem/AchievementPerfectWin.cpp \
                   ../../Classes/AchievementSystem/AchievementFastWin.cpp \
                   ../../Classes/AchievementSystem/AchievementSlowWin.cpp \
                   ../../Classes/AchievementSystem/AchievementPlayedHours.cpp \
                   ../../Classes/AchievementSystem/AchievementSurvival.cpp \
                   ../../Classes/AchievementSystem/AchievementMasterGoldSaver.cpp \
                   ../../Classes/AchievementSystem/AchievementTutorialFinished.cpp \
                   ../../Classes/Model/Army.cpp \
                   ../../Classes/Model/Player.cpp \
                   ../../Classes/Model/PlayerNetworkAI.cpp \
                   ../../Classes/Model/Model.cpp 

COCOS_ROOT = ../..
include $(COCOS_ROOT)/cocos2dx/proj.linux/cocos2dx.mk

SHAREDLIBS += -lcocos2d -lcurl -lcocosdenshion
COCOS_LIBS = $(LIB_DIR)/libcocos2d.so $(LIB_DIR)/libcocosdenshion.so

INCLUDES +=  \
	-I$(COCOS_ROOT)/extensions/ \
	-I$(COCOS_ROOT)/external/ \
	-I$(COCOS_ROOT)/CocosDenshion/include \
	-I$(COCOS_ROOT)/external/chipmunk/include/chipmunk

STATICLIBS += \
	$(STATICLIBS_DIR)/libcurl.a \
	$(LIB_DIR)/libextension.a \
	$(LIB_DIR)/libchipmunk.a


$(TARGET): $(OBJECTS) $(STATICLIBS) $(COCOS_LIBS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_LINK)$(CXX) $(CXXFLAGS) -std=gnu++11 $(OBJECTS) -o $@ $(SHAREDLIBS) $(STATICLIBS)

$(OBJ_DIR)/%.o: %.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) -std=gnu++11 $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) -std=gnu++11 $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
