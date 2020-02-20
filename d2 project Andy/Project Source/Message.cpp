#include "Hack.h"

VOID DeathMessage()
{
	if (ClientReady(FALSE))
	{
		CHAR String[10000], Buffer[10000];
		LPSTR Message[20] = {V_Line1, V_Line2, V_Line3, V_Line4, V_Line5, V_Line6, V_Line7, V_Line8, V_Line9, V_Line10,
			V_Line11, V_Line12, V_Line13, V_Line14, V_Line15, V_Line16, V_Line17, V_Line18, V_Line19, V_Line20};

		for (INT i = 0; i < ArraySize(Message) && i <= 20; i++)
		{
			if (strlen(Message[i]) <= NULL)
				continue;

			if (strcpy(String, Message[i]) && strcpy(Buffer, Message[i]))
			{
				if (strstr(String, "#My Account Name#"))
					strcpy_s(Buffer, ReplaceString(String, "#My Account Name#", D2CLIENT_GetGameInfo()->szAccountName));

				if (strstr(String, "#My Character Name#"))
					strcpy_s(Buffer, ReplaceString(String, "#My Character Name#", D2CLIENT_GetGameInfo()->szCharName));

				if (strstr(String, "#Realm Name#"))
					strcpy_s(Buffer, ReplaceString(String, "#Realm Name#", D2CLIENT_GetGameInfo()->szRealmName));

				if (strstr(String, "#Slain Name#"))
					strcpy_s(Buffer, ReplaceString(String, "#Slain Name#", V_SlainName));

				if (strstr(String, "#Current Song#"))
					strcpy_s(Buffer, ReplaceString(String, "#Current Song#", CurrentSong()));

				if (strstr(String, "#Date#"))
					strcpy_s(Buffer, ReplaceString(String, "#Date#", V_LocalDate));

				if (strstr(String, "#Time#"))
					strcpy_s(Buffer, ReplaceString(String, "#Time#", V_LocalTime));

				if (strstr(String, "#Total Kill Count#"))
					strcpy_s(Buffer, ReplaceString(String, "#Total Kill Count#", V_InjectKills));

				if (strstr(String, "#In-Game Kill Count#"))
					strcpy_s(Buffer, ReplaceString(String, "#In-Game Kill Count#", V_InGameKills));

				if (strstr(String, "#Total Death Count#"))
					strcpy_s(Buffer, ReplaceString(String, "#Total Death Count#", V_InjectDeaths));

				if (strstr(String, "#In-Game Death Count#"))
					strcpy_s(Buffer, ReplaceString(String, "#In-Game Death Count#", V_InGameDeaths));

				if (Say(String))
					SleepEx(V_DeathMessageDelay, TRUE);
			}
		}
	}
}

BOOL FASTCALL Input(WCHAR * Text)
{
	CHAR Buffer[100];
	CHAR Message[100];
	CHAR * Argument[32];

	WideCharToMultiByte(CP_ACP, 0, Text, -1, Buffer, sizeof(Buffer), 0, 0);
	strcpy(Message, Buffer);
	INT Arguments = StringTokenize(Buffer + 1, ' ', Argument, 32);

	if (Buffer[0] == ';')
	{
		if (!_stricmp(Argument[0], "hc"))
		{
			V_HideCommands = !V_HideCommands;
			Print(0, 4, V_HideCommands ? "%s�c0: �c4Commands are hidden" : "%s�c0: �c1Commands aren't hidden", V_Name);
			return FALSE;
		}
	}

	if (V_HideCommands)
		return TRUE;

	if (Buffer[0] == '<')
	{
		if (!_stricmp(Argument[0], "3"))
		{
			Say("  *  *       *  * ");
			Say("*      *   *      *");
			Say("*       * *       *");
			Say(" *       *       *");
			Say("   *           *");
			Say("     *       *");
			Say("       *   *");
			Say("        * *");
			Say("         *");
			return FALSE;
		}
	}

	if (Buffer[0] == '.')
	{
		if (!_stricmp(Argument[0], "0"))
		{
			SYSTEMTIME Time;
			GetLocalTime(&Time);
			sprintf_s(V_LocalDate, "%.2d-%.2d-%.2d", Time.wMonth, Time.wDay, Time.wYear);

			if (Time.wHour < 13 || Time.wHour > 23)
				sprintf_s(V_LocalTime, "%.2d:%.2d:%.2d AM", Time.wHour == 0 ? 12 : Time.wHour, Time.wMinute, Time.wSecond);
			else if (Time.wHour < 24 || Time.wHour > 12)
				sprintf_s(V_LocalTime, "%.2d:%.2d:%.2d PM", Time.wHour == 24 ? 12 : Time.wHour - 12, Time.wMinute, Time.wSecond);

			V_DeathMessageIteration = TRUE;
			return FALSE;
		}

		if (!_stricmp(Argument[0], "1"))
		{
			Say("                     __");
			Say("                    !   !");
			Say("                    !   !");
			Say("                    !   !");
			Say("               ___!   !___");
			Say("          ___!    !   !    !");
			Say("         !     !    !   !    !    _");
			Say("         !     !    !   !    !   /  !");
			Say("         !                    ! /    !");
			Say("         !                    !/    /");
			Say("         !                         /");
			Say("          !                       /");
			Say("           !                    /");
			Say("            !                 /");
			Say("             FUCK YOU");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "2"))
		{
			Say("======================================================");
			Say("===========+++=========+++++++++++++++================");
			Say("===========+++=========+++++++++++++++================");
			Say("===========+++=========+++============================");
			Say("===========+++=========+++============================");
			Say("===========+++=========+++============================");
			Say("===========+++=========+++============================");
			Say("===========+++++++++++++++++++++++++++================");
			Say("===========+++++++++++++++++++++++++++================");
			Say("=======================+++=========+++================");
			Say("=======================+++=========+++================");
			Say("=======================+++=========+++================");
			Say("=======================+++=========+++================");
			Say("===========+++++++++++++++=========+++================");
			Say("===========+++++++++++++++=========+++================");
			Say("======================================================");
			Say("HEIL, MEIN FUHRER. SIEG HEIL!");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "3"))
		{
			Say("      ___");
			Say("     / /  7");
			Say("    (_,_/\\");
			Say("      \\     \\");
			Say("       \\     \\");
			Say("       _\\     \\__");
			Say("      (   \\       )");
			Say("       \\__\\___/");
			Say("   EAT A DICK");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "4"))
		{
			Say("                                       _,---._");
			Say("                                   _,-' _     )");
			Say("                                  (_.-'   _.-'");
			Say("                                _()   _,-'");
			Say("                               (  )  /");
			Say("                                )(  /");
			Say("  ________________ ( /)");
			Say("()__)____________)))))");
			Say("NIGGA GOT SMOKED");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "5"))
		{
			Say("   ,---------------------.");
			Say("  (_/| UNIVERSITY |  ");
			Say("      |         OF         |");
			Say("      |        OWN      |");
			Say("    _|    DIPLOMA   |");
			Say("   (_/_____(*)_____/");
			Say("                 \\");
			Say("                  ))");
			Say("                  ^");
			Say("KID GOT SCHOOLED");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "6"))
		{
			Say("  ,--^-------------,--------,---------,-------^--,");
			Say("  | |||||||||    `--------'          |               O");
			Say("  `+--------------------------------^-----------|");
			Say("    `\\_,----------,  ______________________|");
			Say("      / XXXXXX / `|        /");
			Say("     / XXXXXX /   `\\      /");
			Say("    / XXXXXX /\\______(");
			Say("   / XXXXXX /");
			Say("  / XXXXXX /");
			Say(" (_________(");
			Say("  `----------'");
			Say("SHOT THE FUCK DOWN");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "7"))
		{
			Say("     ____.-.____");
			Say("    [___________]");
			Say("  (d|||||||||||b)");
			Say("  `|||TRASH|||`");
			Say("     |||||||||||");
			Say("     |||||||||||");
			Say("     |||||||||||");
			Say("     |||||||||||");
			Say("     `''''''''''''''''''''''`");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "8"))
		{
			Say("           ________");
			Say("          /________ `.");
			Say("        ,'     ~@~     `.");
			Say("       /         _         \\  \\");
			Say("      /       _| |_       \\  \\");
			Say("      | ~ |_    _| ~  | |");
			Say("      |        |  |        | |");
			Say("      |        |_|        | |");
			Say("      |    _  ___  _     | |");
			Say("      |   |_)  |  |_)    | |");
			Say("      |   | \\ _|_ |      | |");
			Say("      |_____________|");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "9"))
		{
			Say("        _");
			Say("       / /\\");
			Say("      / / /");
			Say("     / / /   _");
			Say("    /_/ /   / /\\");
			Say("    \\ \\ \\  / /  \\");
			Say("     \\ \\ \\/ / /\\ \\");
			Say("  _   \\ \\ \\/ /\\ \\ \\");
			Say("/_/\\   \\_\\  /  \\ \\ \\");
			Say("\\ \\ \\  / /  \\   \\_\\/");
			Say(" \\ \\ \\/ / /\\ \\");
			Say("  \\ \\ \\/ /\\ \\ \\");
			Say("   \\ \\  /  \\ \\ \\");
			Say("    \\_\\/   / / /");
			Say("          / / /");
			Say("         /_/ /");
			Say("         \\_\\/");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "10"))
		{
			Say("                           .-''-.");
			Say("                       .-'`(o) ;");
			Say("                      '-==.     |");
			Say("                           `._...-;-.");
			Say("                            )--""""""   `-.");
			Say("                           /   .        `-.");
			Say("                          /   /      `.    `-.");
			Say("                          |   \\    ;   \\      `-._________");
			Say("                          |    \\    `.`.;          -------`.");
			Say("                           \\    `-.   \\\\\\\\          `---...|");
			Say("                           `.     `-. ```\\.--'._   `---...|");
			Say("                            `-.....7`-.))\\     `-._`-.. /");
			Say("                              `._\\ /   `-`         `-.,'");
			Say("                                 /=(_");
			Say("                              ,^-(_");
			Say("                            ,--' `  ");
			Say("KID GOT SHIT ON BY PIGEON");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "11"))
		{
			Say("                          .-----------------TTTT_-----_______");
			Say("                        /''''''''''(______O] ----------________  \\______/]_");
			Say("     __...---'""""""\\_ --''   Q                                   _____________@");
			Say(" |'''                   ._   _______________=---------""""""""""""""""""""""");
			Say(" |                ..--''|   l L |_l   |");
			Say(" |          ..--''      .  /-___j  '   '");
			Say(" |    ..--''           /  ,          '   '");
			Say(" |--''                /              `    \\");
			Say("                      L__'            \\     -");
			Say("                                         -     '-.");
			Say("                                         '.     /");
			Say("                                           '-./");
			Say("SHOT THE FUCK DOWN");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "12"))
		{
			Say("            .-------------------------------------------------------------------------.");
			Say("           ( ANOTHER KID GETS FUCKING OWNED BY RHINOCEROS )");
			Say("          //'-------------------------------------------------------------------------'");
			Say("         /      , _.-~~-.__            __.,----.");
			Say("      (';    __( )         ~~~'--..--~~         '.");
			Say("(    . \\''..-'  ')|                     .                \\  '.");
			Say("\\. |\\.'                    ;       .  ;                  ;   ;");
			Say("  \\ \\'''   /9)                 '       .  ;                    ;");
			Say("   ; )           )    (        '       .  ;            '        .");
			Say("    )    _  __.-'-._   ;       '       . ,         /\\         ;");
			Say("    '-''''--'      ; ''-. '.    '                _.-(    ''.      (");
			Say("                  ;    \\,)    )--,..--------';'    >   ;     .");
			Say("                   \\   ( |   /               (    /       .     ;");
			Say("     ,   ,          )  | ; .(      .    ,   , )  /         \\    ;");
			Say(",;'-PjP;.';.-.;._/;,;)/;.;.);.;,,;,;,,;,;,;/;;,),;.,;,;,;/,;.,;).,;,");
			Say("RHINOWNED");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "13"))
		{
			Say("       ---_ ......._-_--.");
			Say("      (|\\ /      / /| \\   \\");
			Say("      /  /     .'  -=-'    `.");
			Say("     /  /    .'                )");
			Say("   /  /   .'          _.)    /");
			Say(" / o   o         _.-' /   .'");
			Say(" \\           _.-'    / .'  * |");
			Say("  \\______.-'//    .'.' \\* |");
			Say("    \\|  \\ | //   .'.' _  |*|");
			Say("     `   \\|//  .'.'_ _  |*|");
			Say("      .  .// .'.' | _ _   \\*|");
			Say("      \\`-|\\_/ /  \\ _ _   \\*\\");
			Say("       `/'\\__/     \\ _ _  \\* \\");
			Say("                      \\ _ _  \\* \\");
			Say("                       \\ _ _  \\* \\");
			Say("ANOTHER KID GETS BIT BY THE SNAKE");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "14"))
		{
			Say("    /~~~~~~~~~~~\\");
			Say("  /~  ( )( )( )( )( )( )( )  ~\\");
			Say("  (_)==========(_)");
			Say("   |_________________|");
			Say("FUCK YO' COUCH NIGGA, AFK.");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "15"))
		{
			Say("   .d8888b.    888                                   8888888b.                     8888888b.");
			Say("d88P    Y88b 888                                   888     Y88b                   888     Y88b");
			Say("888        888 888                                   888       888                   888        888");
			Say("888               888    8888b.   88888b.    888     d88P 888     888 888      d88P");
			Say("888               888          ''88b 888  ''88b  8888888P''  888     888  8888888P''");
			Say("888        888 888  .d888888 888   888  888              Y88    88P  888");
			Say("Y88b    d88P 888 888    888 888   888  888                Y8bd8P    888");
			Say("   ''Y8888P''   888  ''Y888888 888   888  888                  Y88P      888");
			Say("OVER WEST");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "16"))
		{
			Say("              /��/)");
			Say("           ,/�../ /");
			Say("          /..../ /");
			Say("   ....../��/'...'/�.`��");
			Say("  /'/.../..../.....:^.��\\");
			Say("  ('(...�...�....�_/'...'/");
			Say("  \\.................'...../");
			Say("   '\\'...\\.........._.��");
			Say("     FUCK YOU");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "17"))
		{
			Say("oooooooooo.     oooooooooooo           .o.            oooooooooo.");
			Say("`888'       `Y8b   `888'           `8         .888.          `888'       `Y8b");
			Say("  888            888   888                       .8""888.          888           888");
			Say("  888            888   888oooo8            .8'  `888.        888           888");
			Say("  888            888   888        ""           .88oo8888.      888           888");
			Say("  888          d88'    888              o   .8'         `888.    888         d88'");
			Say("o888bood8P'      o888ooooood8 o88o     o8888o o888bood8P'");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "18"))
		{
			Say("  .d888888b.    888              888   888b         888   8888888888   8888888b.  ");
			Say("d88P''   ''Y88b  888       o     888   8888b       888   888                 888    ''Y88b");
			Say("888         888   888    d8b    888   88888b     888   888                 888       888");
			Say("888         888   888  d888b  888   888Y88b   888   8888888         888       888");
			Say("888         888   888d88888b888   888   Y88b888   888                 888       888");
			Say("888         888   88888P  Y88888   888     Y88888   888                 888       888");
			Say("Y88b.   .d88P   8888P      Y8888   888       Y8888   888                 888    .d88P");
			Say("  ''Y88888P''     888P          Y888    888        Y888    8888888888   8888888P  ");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "19"))
		{
			Say("  .d888888b.    888              888   888b         888  88888888888 ");
			Say("d88P''   ''Y88b  888       o     888   8888b       888          888         ");
			Say("888         888   888    d8b    888   88888b     888          888         ");
			Say("888         888   888  d888b  888   888Y88b   888          888         ");
			Say("888         888   888d88888b888   888   Y88b888          888         ");
			Say("888         888   88888P  Y88888   888     Y88888          888         ");
			Say("Y88b.   .d88P   8888P      Y8888   888       Y8888          888         ");
			Say("  ''Y88888P''     888P          Y888    888        Y888           888        ");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "20"))
		{
			Say("If You Take Internetting Seriously.");
			Say("Call 1-800-Serious");
			Say("1");
			Say("2");
			Say("3");
			Say("S");
			Say("E");
			Say("R");
			Say("I");
			Say("O");
			Say("U");
			Say("S");
			Say("D");
			Say("You Got Serious'd.");
			Say("Now Go Cry To Mommy About How I Took You Seriously.");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "21"))
		{
			Say("                     SIT");
			Say("                     _,_");
			Say("                    (;;;;)");
			Say("               ,__/a /;\\ ");
			Say("               (__    |;|_ ");
			Say("                '--.    \\;/;)");
			Say("                   @\\ (,;)'\\");
			Say("                    (;;;;)     '._");
			Say("                      \\      /  _ '-.");
			Say("                      | | |  |     `\\ _");
			Say("                      | | |`-.\\  \\  (;;)");
			Say("                     (  (;;)       (;;).' \\ \\");
			Say("                     (  ((_)    (((__)   (;)");
			Say("                      GOOD");
			Say("                      DOG");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "22"))
		{
			Say("  *  *       *  * ");
			Say("*      *   *      *");
			Say("*       * *       *");
			Say(" *       *       *");
			Say("   *           *");
			Say("     *       *");
			Say("       *   *");
			Say("        * *");
			Say("         *");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "23"))
		{
			Say(" _______");
			Say("|     _    |");
			Say("|   |_|  |");
			Say("|       __|");
			Say("|     |enis'd");
			Say("|     |");
			Say("|___|");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "24"))
		{
			Say("8888888888 8888888888P");
			Say("888                           d88P");
			Say("888                         d88P");
			Say("8888888               d88P");
			Say("888                     d88P");
			Say("888                   d88P");
			Say("888                 d88P");
			Say("8888888888 d8888888888");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "25"))
		{
			Say("8888888b.                                                                  888");
			Say("888      Y88b                                                               888");
			Say("888        888                                                               888");
			Say("888      d88P  8888b.     88888b.       .d88b.        .d88888");
			Say("8888888P''           ''88b  888  ''88b   d8P   Y8b  d88''  888");
			Say("888  T88b     .d888888  888    888 88888888  888    888");
			Say("888    T88b   888   888  888  d88P  Y8b.          Y88b  888");
			Say("888      T88b ''Y888888 88888P''        ''Y8888     ''Y88888");
			Say("                                      888                                             ");
			Say("                                      888                                             ");
			Say("                                      888                                             ");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "26"))
		{
			Say(" .'' ''.         .'' '',");
			Say("|    |      /    /");
			Say("|    |    /    /");
			Say("|    |  /    /");
			Say("|    |/    ;----.___ ");
			Say("}     `__/      /     ;");
			Say("|    /`   )     /      /");
			Say("|  /     /___/\\___/\\");
			Say("|/     /               |");
			Say("(     '   \\     '-      |");
			Say(" \\          `.        /");
			Say("  |                 |");
			Say("  |                 |");
			Say("       PEACE     ");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "27"))
		{
			Say("                         _____________");
			Say("                       ( zzzzZZZZZZZ  \\");
			Say("                        `------------------'");
			Say("                                            O");
			Say("               _______                    O       ____");
			Say(" _______(_           |                     o   ,'       `.");
			Say("|:::::::::::::::\\         |                         :  `--.     \\");
			Say("|::::::::::::::::|       |                         (-)     \\    :");
			Say("|::::::::::::::::|       |                       /       @:   |");
			Say("|::::::::::::::::|       |                       `:         :   :");
			Say("|::::::::::::::::|       |                          \\-       ; /");
			Say("|::::::::::::::::|       |                            ''---'=\\");
			Say("|::::::::::::::::|       |              ___       /        `.\\");
			Say("'----------,---'---.---'     ____,:__/`-.:_,-*         :'");
			Say("---------'---------`------`------'  `-.       _,        |");
			Say("----------------------------------'     `---''           |");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "28"))
		{
			Say(" _");
			Say("/_/\\/\\");
			Say("\\_\\  /");
			Say("/_/  \\");
			Say("\\_\\/\\ \\");
			Say("   \\_\\/");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "29"))
		{
			Say("  .d8888888b.      .d8888888b.");
			Say("d88P''      ''Y88b d88P''      ''Y88b");
			Say("888   d8b    888 888   d8b    888");
			Say("888   888    888 888   888    888");
			Say("888   888bd88P 888   888bd88P");
			Say("888   Y8888P''   888   Y8888P''");
			Say("Y88b.           .d8 Y88b.           .d8");
			Say("  ''Y88888888P''   ''Y88888888P''");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "30"))
		{
			Say("        Y888P                            Y888P");
			Say("      Y88o88P                        Y88o88P");
			Say("    Y88b  d88P                    Y88b  d88P");
			Say("  Y88b      d88P                Y88b      d88P");
			Say("                                                               ");
			Say("                         88888888                      ");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "aa"))
		{
			V_Aim = !V_Aim;
			Print(0, 4, V_Aim ? "Aim�c0: �c4Enabled" : "Aim�c0: �c1Disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "af"))
		{
			V_AntiFlash = !V_AntiFlash;
			Print(0, 4, V_AntiFlash ? "Flash�c0: �c4Anti Flash is enabled" : "Flash�c0: �c1Anti Flash is disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "aig"))
		{
			V_AntiItemGlitch = !V_AntiItemGlitch;
			Print(0, 4, V_AntiItemGlitch ? "%s�c0: �c4Anti Flash is enabled" : "%s�c0: �c1Anti Flash is disabled", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "am"))
		{
			V_AntiMinimize = !V_AntiMinimize;
			Print(0, 4, V_AntiMinimize ? "%s�c0: �c4Anti Minimize is enabled" : "%s�c0: �c1Anti Minimize is disabled", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "ap"))
		{
			V_AutoParty = !V_AutoParty;
			Print(0, 4, V_AutoParty ? "%s�c0: �c4Auto Party is enabled" : "%s�c0: �c1Auto Party is disabled", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "aqb"))
		{
			V_AntiQuestBox = !V_AntiQuestBox;
			Print(0, 4, V_AntiQuestBox ? "%s�c0: �c4Anti Quest Box is enabled" : "%s�c0: �c1Anti Quest Box is disabled", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "as"))
		{
			if (Argument[1])
			{
				ChangeStat(STAT_IAS, (atoi(Argument[1]) < 255) ? atoi(Argument[1]) : 255);
				
				if (atoi(Argument[1]) > 0)
					Print(0, 4, "Speed�c0: Increased attack speed by �c3%s%%�c0.", Argument[1]);
				else
					Print(0, 4, "Speed�c0: Decreased attack speed to normal %%");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "bf"))
		{
			V_BlockFakes = !V_BlockFakes;
			Print(0, 4, V_BlockFakes ? "Aim�c0: Blocking fake attacks and casts" : "Aim�c0: �c1Not blocking fake attacks and casts");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "br"))
		{
			if (Argument[1])
			{
				ChangeStat(STAT_FASTERBLOCK, (atoi(Argument[1]) < 255) ? atoi(Argument[1]) : 255);
				
				if (atoi(Argument[1]) > 0)
					Print(0, 4, "Speed�c0: Increased block rate by �c3%s%%", Argument[1]);
				else
					Print(0, 4, "Speed�c0: Decreased block rate to normal %%");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "btp"))
		{
			V_BlockTP = !V_BlockTP;
			Print(0, 4, V_BlockTP ? "Escape�c0: Blocking town portal interaction when towned by life/mana %%" : "Escape�c0: �c1Not blocking town portal interaction when towned by life/mana %%");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "clean"))
		{
			V_Design->Hide = !V_Design->Hide;
			return FALSE;
		}

		if (!_stricmp(Argument[0], "cr"))
		{
			if (Argument[1])
			{
				ChangeStat(STAT_FASTERCAST, (atoi(Argument[1]) < 255) ? atoi(Argument[1]) : 255);
				
				if (atoi(Argument[1]) > 0)
					Print(0, 4, "Speed�c0: Increased cast rate by �c3%s%%", Argument[1]);
				else
					Print(0, 4, "Speed�c0: Decreased cast rate to normal %%");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "cs"))
		{
			if (Argument[1])
			{
				CHAR Temporary[10000];

				if (MaxTime() != INFINITE)
					sprintf_s(Temporary, "/f m �c%s%s%s <%.2d:%.2d:%.2d / %.2d:%.2d:%.2d>", Argument[1], V_CurrentSongPhrase, CurrentSong(), (CurrentTime() / 3600000) % 60, (CurrentTime() / 60000) % 60, (CurrentTime() / 1000) % 60, (MaxTime() / 3600) % 60, (MaxTime() / 60) % 60, MaxTime() % 60);
				else
					sprintf_s(Temporary, "/f m �c%s%s%s <%.2d:%.2d:%.2d / --:--:-->", Argument[1], V_CurrentSongPhrase, CurrentSong(), (CurrentTime() / 3600000) % 60, (CurrentTime() / 60000) % 60, (CurrentTime() / 1000) % 60);

				BNCLIENT_SendBNMessage(Temporary);
				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "cube"))
		{
			if (!GetUIVar(UI_CUBE))
			{
				BYTE Packet[2];
				Packet[0] = 0x77;
				Packet[1] = 0x15;
				D2NET_ReceivePacket(Packet, 2);
				V_CubeOrStashOpened = TRUE;
			}

			else
			{
				D2CLIENT_SetUIVar(UI_CUBE, 1, 0);
				return FALSE;
			}

			return FALSE;
		}

		if (!_stricmp(Argument[0], "dg"))
		{
			if (GetUnitStat(Me, STAT_GOLD) <= 0)
			{
				Print(0, 4, "%s�c0: �c1No gold found in inventory", V_Name);
				return FALSE;
			}

			if (Argument[1] && GetUnitStat(Me, STAT_GOLD) > 0)
			{
				V_GoldAmount = atoi(Argument[1]);
				V_GoldDrop = !V_GoldDrop;

				if (V_GoldDrop)
					Print(0, 4, "%s�c0: Dropping %s gold", V_Name, Argument[1]);
				else
					Print(0, 4, "%s�c0: �c1Stopped dropping gold", V_Name);

				return FALSE;
			}

			V_GoldDrop = FALSE;
			Print(0, 4, "%s�c0: �c1Stopped dropping gold", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "dm"))
		{
			V_DeathMessage = !V_DeathMessage;
			Print(0, 4, V_DeathMessage ? "Death Message�c0: �c4Enabled" : "Death Message�c0: �c1Disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "eb"))
		{
			V_Enchant = !V_Enchant;
			Print(0, 4, V_Enchant ? "Enchant�c0: �c4Enabled" : "Enchant�c0: �c1Disabled");

			if (V_Enchant && V_EnchantNote)
				V_EnchantNoteIteration = TRUE;
			else if (!V_Enchant && V_EnchantNote)
				Say("%s's Enchant Bot is disabled", V_Name);

			return FALSE;
		}

		if (!_stricmp(Argument[0], "ebn"))
		{
			V_EnchantNote = !V_EnchantNote;
			Print(0, 4, V_EnchantNote ? "Enchant�c0: �c4Note is enabled" : "Enchant�c0: �c1Note is disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "el"))
		{
			if (Argument[1])
			{
				V_LifeExitPercent = atoi(Argument[1]);

				if (V_LifeExitPercent > 0)
					Print(0, 4, "Escape�c0: �c4Escaping out of game at �c1%s%% life", Argument[1]);
				else
					Print(0, 4, "Escape�c0: Escaping out of game at �c1life %% �c0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "em"))
		{
			if (Argument[1])
			{
				V_ManaExitPercent = atoi(Argument[1]);

				if (V_ManaExitPercent > 0)
					Print(0, 4, "Escape�c0: �c4Escaping out of game at �c3%s%% mana", Argument[1]);
				else
					Print(0, 4, "Escape�c0: Escaping out of game at �c3mana %% �c0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "fake"))
		{
			if (Argument[2])
			{
				SYSTEMTIME Time;
				GetLocalTime(&Time);
				sprintf_s(V_LocalDate, "%.2d-%.2d-%.2d", Time.wMonth, Time.wDay, Time.wYear);

				if (Time.wHour < 13 || Time.wHour > 23)
					sprintf_s(V_LocalTime, "%.2d:%.2d:%.2d AM", Time.wHour == 0 ? 12 : Time.wHour, Time.wMinute, Time.wSecond);
				else if (Time.wHour < 24 || Time.wHour > 12)
					sprintf_s(V_LocalTime, "%.2d:%.2d:%.2d PM", Time.wHour == 24 ? 12 : Time.wHour - 12, Time.wMinute, Time.wSecond);

				if (Print(0, 4, "%s was slain by %s", Argument[1], Me->pPlayerData->szName))
					Print(0, 4, "%s(%s) left our world. Diablo's minions weaken.", Argument[1], Argument[2]);

				if (V_DeathMessage)
				{
					strcpy(V_SlainName, Argument[1]);
					V_DeathMessageIteration = TRUE;
				}

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "fc"))
		{
			for (INT i = 0; i < V_Players.GetSize(); i++)
			{
				if (V_Players[i]->Flashing)
				{
					V_Players[i]->Flashing = FALSE;
					Print(0, 4, "Flash�c0: �c4Stopped flashing �c1%s", V_Players[i]->PlayerName);
				}
			}

			return FALSE;
		}

		if (!_stricmp(Argument[0], "fd"))
		{
			if (Argument[1])
			{
				V_FlashDelay = atoi(Argument[1]);

				if (V_FlashDelay > 0)
					Print(0, 4, "Flash�c0: Delay set to �c3%d milliseconds", Argument[1]);
				else
					Print(0, 4, "Flash�c0: �c1Disabled");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "fm"))
		{
			if (Argument[2])
			{
				CHAR Temporary[500];
				sprintf_s(Temporary, "/f m �c%s%s", Argument[1], Message + 6);
				BNCLIENT_SendBNMessage(Temporary);
				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "ftp"))
		{
			V_FastTP = !V_FastTP;
			Print(0, 4, V_FastTP ? "Escape�c0: �c4Fast Town Portal is enabled" : "Escape�c0: �c1Fast Town Portal is disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "hack"))
		{
			if (!GetSkill(D2S_CONFUSE))
				SetSkill(D2S_CONFUSE, FALSE);
			else
				SetSkill(D2S_HOLYSHIELD, FALSE);

			return FALSE;
		}

		if (!_stricmp(Argument[0], "hp"))
		{
			if (Argument[1])
			{
				V_LifePotPercent = atoi(Argument[1]);

				if (V_LifePotPercent > 0)
					Print(0, 4, "Potion�c0: �c4Drinking health potions at �c1%s%% life", Argument[1]);
				else
					Print(0, 4, "Potion�c0: Drinking health potions at �c1life %% �c0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "hr"))
		{
			if (Argument[1])
			{
				ChangeStat(STAT_FASTERHITRECOVERY, (atoi(Argument[1]) < 255) ? atoi(Argument[1]) : 255);
				
				if (atoi(Argument[1]) > 0)
					Print(0, 4, "Speed�c0: Increased hit recovery rate by �c3%s%%", Argument[1]);
				else
					Print(0, 4, "Speed�c0: Decreased hit recovery rate to �c1normal %%");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "load"))
		{
			Load();
			Print(0, 4, "%s�c0: �c4Loaded configuration", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "mh"))
		{
			BOOL Map = V_AutoRevealAutomap;

			if (V_AutoRevealAutomap) V_AutoRevealAutomap = FALSE;
			if (V_Reveal) V_Reveal->RevealAutomap();
			if (!V_AutoRevealAutomap) V_AutoRevealAutomap = Map;

			Print(0, 4, "Reveal�c0: �c4Revealed Act %d", Me->pAct->dwAct + 1);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "mp"))
		{
			if (Argument[1])
			{
				V_ManaPotPercent = atoi(Argument[1]);

				if (V_ManaPotPercent > 0)
					Print(0, 4, "Potion�c0: �c4Drinking mana potions at �c3%s%% mana", Argument[1]);
				else
					Print(0, 4, "Potion�c0: Drinking mana potions at �c3mana %% �c0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "next"))
		{
			NextSong();
			return FALSE;
		}

		if (!_stricmp(Argument[0], "ots"))
		{
			V_OutTownSelect = !V_OutTownSelect;
			Print(0, 4, V_OutTownSelect ? "%s�c0: �c4Players out of town are selectable" : "%s�c0: �c1Players out of town are unselectable", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "pause"))
		{
			Pause();
			return FALSE;
		}

		if (!_stricmp(Argument[0], "play"))
		{
			Play();
			return FALSE;
		}

		if (!_stricmp(Argument[0], "prev"))
		{
			PreviousSong();
			return FALSE;
		}

		if (!_stricmp(Argument[0], "rl"))
		{
			if (Argument[1])
			{
				V_LifeJuvPercent = atoi(Argument[1]);

				if (V_LifeJuvPercent > 0)
					Print(0, 4, "Potion�c0: �c4Drinking rejuvenation potions at �c1%s%% life", Argument[1]);
				else
					Print(0, 4, "Potion�c0: Drinking rejuvenation potions at �c1life %% �c0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "rm"))
		{
			if (Argument[1])
			{
				V_ManaJuvPercent = atoi(Argument[1]);

				if (V_ManaJuvPercent > 0)
					Print(0, 4, "Potion�c0: �c4Drinking rejuvenation potions at �c3%s%% mana", Argument[1]);
				else
					Print(0, 4, "Potion�c0: Drinking rejuvenation potions at �c3mana %% �c0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "rw"))
		{
			if (Argument[1])
			{
				ChangeStat(STAT_FASTERRUNWALK, (atoi(Argument[1]) < 255) ? atoi(Argument[1]) : 255);
				
				if (atoi(Argument[1]) > 0)
					Print(0, 4, "Speed�c0: Increased run/walk speed by �c3%s%%", Argument[1]);
				else
					Print(0, 4, "Speed�c0: Decreased run/walk speed to normal %%");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "save"))
		{
			Save();
			Print(1, 4, "%s�c0: �c4Saved configuration");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "sp"))
		{
			V_ProtectItemSocket = !V_ProtectItemSocket;
			Print(0, 4, V_ProtectItemSocket ? "Reveal�c0: �c4Item socket protection enabled" : "Reveal�c0: �c1Item socket protection disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "ss"))
		{
			V_Screenshot = !V_Screenshot;
			Print(0, 4, V_Screenshot ? "Design�c0: Hide on screenshot enabled" : "Design�c0: �c1Hide on screenshot disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "stash"))
		{
			if (!GetUIVar(UI_STASH))
			{
				BYTE Packet[2];
				Packet[0] = 0x77;
				Packet[1] = 0x10;
				D2NET_ReceivePacket(Packet, 2);
				V_CubeOrStashOpened = TRUE;
			}

			else
			{
				D2CLIENT_SetUIVar(UI_STASH, 1, 0);
				return FALSE;
			}

			return FALSE;
		}

		if (!_stricmp(Argument[0], "stop"))
		{
			Stop();
			return FALSE;
		}

		if (!_stricmp(Argument[0], "ta"))
		{
			V_TPWhenAttacked = !V_TPWhenAttacked;
			
			if (V_TPWhenAttacked)
				Print(0, 4, "Escape�c0: �c4Escaping to town when attacked");
			else
				Print(0, 4, "Escape�c0: �c1Escaping to town when attacked is off");

			return FALSE;
		}

		if (!_stricmp(Argument[0], "th"))
		{
			V_TPWhenHostiled = !V_TPWhenHostiled;
			
			if (V_TPWhenHostiled)
				Print(0, 4, "Escape�c0: �c4Escaping to town when hostiled");
			else
				Print(0, 4, "Escape�c0: �c1Escaping to town when hostiled is off");

			return FALSE;
		}

		if (!_stricmp(Argument[0], "tl"))
		{
			if (Argument[1])
			{
				V_LifeTownPercent = atoi(Argument[1]);

				if (V_LifeTownPercent > 0)
					Print(0, 4, "Escape�c0: �c4Escaping to town at �c1%s%% life", Argument[1]);
				else
					Print(0, 4, "Escape�c0: Escaping to town at �c1life %% �c0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "tm"))
		{
			if (Argument[1])
			{
				V_ManaTownPercent = atoi(Argument[1]);

				if (V_ManaTownPercent > 0)
					Print(0, 4, "Escape�c0: �c4Escaping to town at �c3%s%% mana", Argument[1]);
				else
					Print(0, 4, "Escape�c0: �c0Escaping to town at �c3mana %% �c0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "unload"))
		{
			Detach();
			Print(0, 19, "%s is unloaded, restart Diablo II to reload %s", V_Name, V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "vbs"))
		{
			V_ViewItemBasicStats = !V_ViewItemBasicStats;
			Print(0, 4, V_ViewItemBasicStats ? "Reveal�c0: �c4Viewing basic stats of items is enabled" : "Reveal�c0: �c1Viewing basic stats of items is disabled");
			return FALSE;
		}

		/*if (!_stricmp(Argument[0], "vs"))
		{
			V_ViewSocketables = !V_ViewSocketables;
			Print(0, 4, V_ViewSocketables ? "Reveal�c0: �c4Viewing socketables of items is enabled" : "Reveal�c0: �c1Viewing socketables of items is disabled");
			return FALSE;
		}*/

		if (!_stricmp(Argument[0], "vp"))
		{
			if (Argument[1])
			{
				ChangeStat(STAT_VELOCITYPERCENT, (atoi(Argument[1]) < 255) ? atoi(Argument[1]) : 255);
				
				if (atoi(Argument[1]) > 100)
					Print(0, 4, "Speed�c0: Increased velocity by �c3%s%%", Argument[1]);
				else if (atoi(Argument[1]) == 100)
					Print(0, 4, "Speed�c0: Decreased velocity to normal %%");
				else if (atoi(Argument[1]) < 100)
					Print(0, 4, "Speed�c0: Decreased velocity by �c3%s%%", Argument[1]);

				return FALSE;
			}
		}
	}

	if (Buffer[0] == '?')
	{
		if (!_stricmp(Argument[0], "date"))
		{
			if (Say("?%s", Argument[0]))
			{
				CHAR LocalTime[50];
				SYSTEMTIME Time;
				GetLocalTime(&Time);
				
				sprintf_s(LocalTime, "%.2d-%.2d-%.2d", Time.wMonth, Time.wDay, Time.wYear);
				Say(LocalTime);
			}

			return FALSE;
		}

		if (!_stricmp(Argument[0], "ping"))
		{
			if (Say("?%s", Argument[0]))
				Say("Current FPS: %d, Skip: %d, Ping: %d", *p_D2CLIENT_FPS, *p_D2CLIENT_Skip, *p_D2CLIENT_Ping);

			return FALSE;
		}

		if (!_stricmp(Argument[0], "score"))
		{
			if (Say("?%s", Argument[0]))
				Say("Global kills: %i, Global Deaths: %i, Kills in this game: %i, Deaths in this game: %i",
				V_InjectKillCount, V_InjectDeathCount, V_InGameKillCount, V_InGameDeathCount);

			return FALSE;
		}

		if (!_stricmp(Argument[0], "time"))
		{
			if (Say("?%s", Argument[0]))
			{
				CHAR LocalTime[50];
				SYSTEMTIME Time;
				GetLocalTime(&Time);

				if (Time.wHour < 13 || Time.wHour > 23 && strlen(LocalTime) <= 0)
					sprintf_s(LocalTime, "%.2d:%.2d:%.2d AM", Time.wHour == 0 ? 12 : Time.wHour, Time.wMinute, Time.wSecond);
				else if (Time.wHour < 24 || Time.wHour > 12 && strlen(LocalTime) <= 0)
					sprintf_s(LocalTime, "%.2d:%.2d:%.2d PM", Time.wHour == 24 ? 12 : Time.wHour - 12, Time.wMinute, Time.wSecond);

				Say(LocalTime);
			}

			return FALSE;
		}

		if (!_stricmp(Argument[0], "ver"))
		{
			if (Say("?%s", Argument[0]))
				Say("Currently running: %s Version %.1f by ANdy", V_Name, V_Version);

			return FALSE;
		}
	}

	return TRUE;
}