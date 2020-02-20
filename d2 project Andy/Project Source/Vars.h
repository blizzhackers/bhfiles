#ifdef VARS

#define ARRAY(Type, Name)		CArrayEx<Type, Type> V_##Name;
#define KEY(Name)				INT K_##Name;
#define TOGGLE(Name)			BOOL V_##Name; INT K_##Name;
#define VAR(Type, Name)			Type V##_##Name;

#else

#define ARRAY(Type, Name)		extern CArrayEx<Type, Type> V_##Name;
#define KEY(Name)				extern INT K_##Name;
#define TOGGLE(Name)			extern BOOL V_##Name; extern INT K_##Name;
#define VAR(Type, Name)			extern Type V_##Name;

#endif

VAR(HINSTANCE, DLL)
VAR(BOOL, Game)
VAR(INT, PatchInGame)
VAR(BOOL, PatchOutGame)
VAR(DWORD, GameTimer)

VAR(LPSTR, Name)
VAR(FLOAT, Version)

VAR(CHAR, FileNamePath[1000])
VAR(WNDPROC, OldWndProc)

VAR(BOOL, AntiMinimize)
VAR(BOOL, CubeOrStashOpened)
VAR(BOOL, HideCommands)
VAR(BOOL, HostileImage)

KEY(ControlHotkey)
KEY(LoadConfiguration)

//Aim
ARRAY(PTARGETINFO, Players)

KEY(AttackMyselfLeft)
KEY(AttackMyselfRight)
KEY(AttackNearestTargetLeft)
KEY(AttackNearestTargetRight)
KEY(AttackTargetLeft)
KEY(AttackTargetRight)
KEY(Blind)
KEY(NextTarget)
KEY(PreviousTarget)
KEY(SetKickLeft)
KEY(SetKickRight)

TOGGLE(AutoBlind)
TOGGLE(AutoKB)

VAR(BOOL, Aim)
VAR(BOOL, BlockFakes)
VAR(BOOL, HostilesOnly)
VAR(BOOL, KB)
VAR(BOOL, Kick)
VAR(BOOL, LeftAttack)
VAR(BOOL, Reblind)

VAR(INT, Target)
VAR(INT, Blind)

VAR(DWORD, AutoBlindDistance)
VAR(DWORD, AutoKBDistance)

//Auto Party
VAR(BOOL, AutoParty)

//Copy & Paste
VAR(BOOL, CopyPaste)
VAR(LPCONTROL, EditBox)

//Death Message
VAR(BOOL, DeathMessage)
VAR(BOOL, DeathMessageIteration)

VAR(CHAR, LocalTime[50])
VAR(CHAR, LocalDate[50])

VAR(CHAR, InjectKills[1000])
VAR(CHAR, InGameKills[1000])
VAR(CHAR, InjectDeaths[1000])
VAR(CHAR, InGameDeaths[1000])

VAR(INT, InjectKillCount)
VAR(INT, InGameKillCount)
VAR(INT, InjectDeathCount)
VAR(INT, InGameDeathCount)

VAR(DWORD, DeathMessageDelay)

VAR(CHAR, Line1[1000])
VAR(CHAR, Line2[1000])
VAR(CHAR, Line3[1000])
VAR(CHAR, Line4[1000])
VAR(CHAR, Line5[1000])
VAR(CHAR, Line6[1000])
VAR(CHAR, Line7[1000])
VAR(CHAR, Line8[1000])
VAR(CHAR, Line9[1000])
VAR(CHAR, Line10[1000])
VAR(CHAR, Line11[1000])
VAR(CHAR, Line12[1000])
VAR(CHAR, Line13[1000])
VAR(CHAR, Line14[1000])
VAR(CHAR, Line15[1000])
VAR(CHAR, Line16[1000])
VAR(CHAR, Line17[1000])
VAR(CHAR, Line18[1000])
VAR(CHAR, Line19[1000])
VAR(CHAR, Line20[1000])

VAR(CHAR, SlainName[30])

//Design
KEY(Clean)
KEY(Options)
KEY(Stats)

VAR(Design *, Design)

VAR(BOOL, Mouse)
VAR(BOOL, Orbs)
VAR(BOOL, PlayerLifePercent)
VAR(BOOL, PlayerLifeBar)
VAR(BOOL, Screenshot)
VAR(BOOL, Vectors)

VAR(INT, HostilePlayerColor)
VAR(INT, TargetPlayerColor)

VAR(BOOL, TargetVector)
VAR(INT, TargetVectorColor)

VAR(BOOL, TargetClick)
VAR(INT, TargetClickColor)

VAR(CHAR, AutomapInfoGameNameText[0x100])
VAR(CHAR, AutomapInfoGamePasswordText[0x100])
VAR(CHAR, AutomapInfoLevelText[0x100])
VAR(CHAR, AutomapInfoDifficultyText[0x100])
VAR(CHAR, AutomapInfoIPText[0x100])
VAR(CHAR, AutomapInfoLocalTimeText[0x100])
VAR(CHAR, AutomapInfoGameTimerText[0x100])
VAR(CHAR, AutomapInfoFPSText[0x100])
VAR(CHAR, AutomapInfoSkipText[0x100])
VAR(CHAR, AutomapInfoPingText[0x100])

VAR(INT, AutomapInfoGameNameColor)
VAR(INT, AutomapInfoGamePasswordColor)
VAR(INT, AutomapInfoLevelColor)
VAR(INT, AutomapInfoDifficultyColor)
VAR(INT, AutomapInfoIPColor)
VAR(INT, AutomapInfoLocalTimeColor)
VAR(INT, AutomapInfoGameTimerColor)
VAR(INT, AutomapInfoFPSColor)
VAR(INT, AutomapInfoSkipColor)
VAR(INT, AutomapInfoPingColor)

VAR(INT, AutomapInfoGameNameFont)
VAR(INT, AutomapInfoGamePasswordFont)
VAR(INT, AutomapInfoLevelFont)
VAR(INT, AutomapInfoDifficultyFont)
VAR(INT, AutomapInfoIPFont)
VAR(INT, AutomapInfoLocalTimeFont)
VAR(INT, AutomapInfoGameTimerFont)
VAR(INT, AutomapInfoFPSFont)
VAR(INT, AutomapInfoSkipFont)
VAR(INT, AutomapInfoPingFont)

VAR(BOOL, EscapeInfo)
VAR(INT, EscapeInfoX)
VAR(INT, EscapeInfoY)
VAR(INT, EscapeInfoTitleColor)
VAR(INT, EscapeInfoExitLifeColor)
VAR(INT, EscapeInfoExitManaColor)
VAR(INT, EscapeInfoTownLifeColor)
VAR(INT, EscapeInfoTownManaColor)

VAR(BOOL, ExpInfo)
VAR(INT, ExpInfoX)
VAR(INT, ExpInfoY)
VAR(INT, ExpInfoTitleColor)
VAR(INT, ExpInfoLevelColor)
VAR(INT, ExpInfoExperienceColor)
VAR(INT, ExpInfoExperienceNeededColor)
VAR(INT, ExpInfoExperienceGainedColor)

VAR(INT, LifeOrbColor)
VAR(INT, LifeOrbFont)
VAR(INT, ManaOrbColor)
VAR(INT, ManaOrbFont)

VAR(BOOL, Options)
VAR(INT, OptionTitleColor)
VAR(INT, OptionTitleFont)
VAR(INT, OptionVariableColor)
VAR(INT, OptionVariableFont)
VAR(INT, OptionCloseColor)
VAR(INT, OptionCloseFont)

VAR(BOOL, PotionInfo)
VAR(INT, PotionInfoX)
VAR(INT, PotionInfoY)
VAR(INT, PotionInfoTitleColor)
VAR(INT, PotionInfoLifePotColor)
VAR(INT, PotionInfoManaPotColor)
VAR(INT, PotionInfoLifeJuvColor)
VAR(INT, PotionInfoManaJuvColor)

VAR(BOOL, StateList)
VAR(INT, StateListX)
VAR(INT, StateListY)
VAR(INT, StateListTitleColor)
VAR(INT, StateListStateColor)

VAR(BOOL, Supplies)
VAR(INT, SuppliesX)
VAR(INT, SuppliesY)
VAR(INT, SuppliesTitleColor)
VAR(INT, SuppliesHealPotColor)
VAR(INT, SuppliesManaPotColor)
VAR(INT, SuppliesJuvPotColor)
VAR(INT, SuppliesTPColor)
VAR(INT, SuppliesGoldColor)

VAR(BOOL, TargetInfo)
VAR(INT, TargetInfoX)
VAR(INT, TargetInfoY)
VAR(INT, TargetInfoTitleColor)
VAR(INT, TargetInfoNameColor)
VAR(INT, TargetInfoClassColor)
VAR(INT, TargetInfoLevelColor)
VAR(INT, TargetInfoLifeColor)

VAR(BOOL, TargetList)
VAR(INT, TargetListNonTargettedColor)
VAR(INT, TargetListTitleColor)
VAR(INT, TargetListTargettedColor)
VAR(INT, TargetListX)
VAR(INT, TargetListY)

VAR(BOOL, MusicPlayer)
VAR(INT, MusicPlayerX)
VAR(INT, MusicPlayerY)
VAR(INT, MusicPlayerTitleColor)
VAR(INT, MusicPlayerSongColor)
VAR(INT, MusicPlayerSongLength)
VAR(INT, MusicPlayerTimeColor)

//Drop Gold
VAR(BOOL, GoldDrop)
VAR(DWORD, GoldAmount)

//Enchant
ARRAY(LPENCHANTINFO, EnchantQueue)

VAR(BOOL, Enchant)
VAR(BOOL, EnchantNote)
VAR(BOOL, EnchantNoteIteration)
VAR(BOOL, EnchantWhisper)

VAR(CHAR, EnchantPlayer[1000])
VAR(CHAR, EnchantMercenary[1000])
VAR(CHAR, EnchantAll[1000])

//Escape
KEY(Exit)
KEY(Town)
KEY(TP)

VAR(BOOL, BlockTP)
VAR(BOOL, FastTP)
VAR(BOOL, NoTPDelay)
VAR(BOOL, TP)
VAR(BOOL, TPWhenHostiled)
VAR(BOOL, TPWhenAttacked)

VAR(DWORD, TPIntervalTime)
VAR(DWORD, TPInteractLast)
VAR(DWORD, TPLast)

VAR(DWORD, LifeExitPercent)
VAR(DWORD, ManaExitPercent)

VAR(DWORD, LifeTownPercent)
VAR(DWORD, ManaTownPercent)

//Experience
VAR(DWORD, StartingExperience)

//Flash
VAR(BOOL, AntiFlash)

VAR(DWORD, FlashDelay)
VAR(DWORD, FlashLast)

TOGGLE(Flash)

//Game Filter
VAR(LPCONTROL, FilterBox)
VAR(list<LPGAMELISTENTRY>, GameList)
VAR(BOOL, GameFilter)

//Miscellaneous
VAR(BOOL, AntiItemGlitch)
VAR(BOOL, AntiQuestBox)
VAR(BOOL, AutomapOnJoin)
VAR(BOOL, MonsterDeathAnimation)

VAR(BOOL, KillLog)
VAR(BOOL, LogInGameMessages)
VAR(BOOL, ModifiedJoinMessage)

VAR(BOOL, OutTownSelect)
VAR(BOOL, PlayerInTown)

VAR(BOOL, RememberGameName)
VAR(BOOL, RememberGamePassword)

VAR(WCHAR, GameName[50])
VAR(WCHAR, GamePassword[50])

VAR(INT, ConsoleDelay)
VAR(INT, ConsoleX)
VAR(INT, ConsoleY)

//Notification
VAR(BOOL, Potion)
VAR(BOOL, Quests)

VAR(BOOL, LifePot)
VAR(BOOL, ManaPot)
VAR(BOOL, LifeJuv)
VAR(BOOL, ManaJuv)

//Potion
VAR(DWORD, LifePotDelay)
VAR(DWORD, LifePotLast)
VAR(DWORD, LifePotPercent)

VAR(DWORD, LifeJuvDelay)
VAR(DWORD, LifeJuvLast)
VAR(DWORD, LifeJuvPercent)

VAR(DWORD, ManaPotDelay)
VAR(DWORD, ManaPotLast)
VAR(DWORD, ManaPotPercent)

VAR(DWORD, ManaJuvDelay)
VAR(DWORD, ManaJuvLast)
VAR(DWORD, ManaJuvPercent)

VAR(DWORD, MercJuvDelay)
VAR(DWORD, MercJuvLast)
VAR(DWORD, MercJuvPercent)

VAR(DWORD, MercPotDelay)
VAR(DWORD, MercPotLast)
VAR(DWORD, MercPotPercent)

//Reveal
TOGGLE(ProtectItemSocket)
TOGGLE(ViewItemBasicStats)
TOGGLE(ViewSocketables)

//Console
TOGGLE(ConsoleLogs)

VAR(BOOL, AntiShake)
VAR(BOOL, AntiWeather)
VAR(BOOL, AutoRevealAutomap)
VAR(BOOL, Ethereal)
VAR(BOOL, FullLightRadius)
VAR(BOOL, ItemLevel)
VAR(BOOL, LevelNames)
VAR(BOOL, MiniShrine)
VAR(BOOL, Missiles)
VAR(BOOL, Monsters)
VAR(BOOL, MonsterLifeBarResists)
VAR(BOOL, MonsterLifeBarLevel)
VAR(BOOL, MonsterLifeBarTC)
VAR(BOOL, MonsterResists)
VAR(BOOL, MonsterNames)
VAR(BOOL, ShrineImages)
VAR(BOOL, Sockets)
VAR(BOOL, ViewSocketable)

VAR(BOOL, MonsterLifeBarNameOn)
VAR(BOOL, ViewSocketableMonsterLifeBarFlag)

VAR(BYTE, MonsterTCs[1000])
VAR(BYTE, SuperUniqueTCs[100])

VAR(WCHAR, MonsterLifeBarName[0x400])
VAR(WCHAR, MonsterDesc[0x800])

VAR(LPCELLFILE, MyShrineCells[2][TOTALSHRINES])
VAR(LPCELLFILE, WaypointCell)
VAR(LPCELLFILE, MinimapObjectCell)

VAR(MYAUTOMAPLAYERMAN, MyAutoMapLayer[100])

VAR(INT, LevelNamesFontColor)
VAR(CHAR, StaffTombIndicator[0x40])

VAR(BYTE, LifeBarTransparency)
VAR(BYTE, LifeBarColor)

VAR(INT, HostileSummonColor)
VAR(INT, PartiedSummonColor)
VAR(INT, NeutralSummonColor)
VAR(INT, MySummonColor)

VAR(INT, HostileColor)
VAR(INT, MyColor)
VAR(INT, NeutralColor)
VAR(INT, PartyColor)

VAR(INT, BossColor)
VAR(INT, BossNameColor)
VAR(INT, ChampionColor)
VAR(INT, MinionColor)
VAR(INT, NormalColor)

VAR(Reveal *, Reveal)

VAR(WCHAR *, MonsterDescComma)

VAR(LPUNITANY, ViewingUnit)
KEY(ViewInventory)

//Speed
VAR(BOOL, SpeedAttack)
VAR(BOOL, SpeedCast)

VAR(INT, AttackFrames)
VAR(INT, CastFrames)

VAR(INT, AttackSpeed)
VAR(INT, BlockRate)
VAR(INT, CastRate)
VAR(INT, HitRecoveryRate)
VAR(INT, RunWalkSpeed)
VAR(INT, VelocitySpeed)

//States
VAR(BOOL, DelayedChicken)

VAR(DWORD, AmplifyDamageTimer)
VAR(DWORD, BattleCommandTimer)
VAR(DWORD, BattleCryTimer)
VAR(DWORD, BattleOrdersTimer)
VAR(DWORD, BoneArmorTimer)
VAR(DWORD, BurstOfSpeedTimer)
VAR(DWORD, ChillingArmorTimer)
VAR(DWORD, ColdTimer)
VAR(DWORD, ConvictedTimer)
VAR(DWORD, CycloneArmorTimer)
VAR(DWORD, DecrepifyTimer)
VAR(DWORD, EnchantTimer)
VAR(DWORD, EnergyShieldTimer)
VAR(DWORD, FadeTimer)
VAR(DWORD, FrozenTimer)
VAR(DWORD, FrozenArmorTimer)
VAR(DWORD, HolyFrozenTimer)
VAR(DWORD, HolyShieldTimer)
VAR(DWORD, InnerSightTimer)
VAR(DWORD, IronMaidenTimer)
VAR(DWORD, LifeTapTimer)
VAR(DWORD, LowerResistTimer)
VAR(DWORD, OakSageTimer)
VAR(DWORD, OpenWoundsTimer)
VAR(DWORD, PoisonedTimer)
VAR(DWORD, ShiverArmorTimer)
VAR(DWORD, ShoutTimer)
VAR(DWORD, SlowedTimer)
VAR(DWORD, TerrorTimer)
VAR(DWORD, ThunderStormTimer)
VAR(DWORD, VenomTimer)

//Teleport
ARRAY(POINT, TeleportQueue)

KEY(TeleHost)
TOGGLE(TeleWalk)

VAR(BOOL, FastTeleport)
VAR(BOOL, FarcastTeleport)

//Threads
VAR(THREAD, PrimaryThread)
ARRAY(HANDLE, Threads)

//Vectors
ARRAY(LPVECTOR, CurrentVectors)

//Console
//ARRAY(CONSOLEMESSAGE, List)

KEY(NextLevelVector)
KEY(PreviousLevelVector)
KEY(QuestVector)
KEY(WaypointVector)

VAR(INT, ActiveVectorColor)
VAR(INT, NextLevelVectorColor)
VAR(INT, PreviousLevelVectorColor)
VAR(INT, QuestVectorColor)
VAR(INT, WaypointVectorColor)

VAR(DWORD, LastLevel)
VAR(LPVECTOR, CurrentVector)

//Winamp
KEY(CurrentSong)
KEY(NextSong)
KEY(PreviousSong)
KEY(PlaySong)
KEY(PauseSong)
KEY(StopSong)
KEY(FastForwardSong)
KEY(RewindSong)
KEY(HigherSongVolume)
KEY(LowerSongVolume)

VAR(CHAR, CurrentSongPhrase[1000])

#undef VARS
