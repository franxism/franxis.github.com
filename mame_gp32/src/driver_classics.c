#include "driver.h"

/* "Pacman hardware" games */
extern struct GameDriver pacman_driver;
extern struct GameDriver pacmod_driver;
extern struct GameDriver namcopac_driver;
extern struct GameDriver pacmanjp_driver;
extern struct GameDriver hangly_driver;
extern struct GameDriver puckman_driver;
extern struct GameDriver piranha_driver;
extern struct GameDriver pacplus_driver;
extern struct GameDriver mspacman_driver;
extern struct GameDriver mspacatk_driver;
extern struct GameDriver crush_driver;
extern struct GameDriver eyes_driver;
extern struct GameDriver mrtnt_driver;
extern struct GameDriver ponpoko_driver;
extern struct GameDriver lizwiz_driver;
extern struct GameDriver theglob_driver;
extern struct GameDriver beastf_driver;
extern struct GameDriver maketrax_driver;
extern struct GameDriver pengo_driver;
extern struct GameDriver pengo2_driver;
extern struct GameDriver pengo2u_driver;
extern struct GameDriver penta_driver;
extern struct GameDriver jrpacman_driver;

/* "Galaxian hardware" games */
extern struct GameDriver galaxian_driver;
extern struct GameDriver galmidw_driver;
extern struct GameDriver galnamco_driver;
extern struct GameDriver superg_driver;
extern struct GameDriver galapx_driver;
extern struct GameDriver galap1_driver;
extern struct GameDriver galap4_driver;
extern struct GameDriver galturbo_driver;
extern struct GameDriver pisces_driver;
extern struct GameDriver japirem_driver;
extern struct GameDriver uniwars_driver;
extern struct GameDriver warofbug_driver;
extern struct GameDriver redufo_driver;
extern struct GameDriver pacmanbl_driver;
extern struct GameDriver zigzag_driver;
extern struct GameDriver zigzag2_driver;
extern struct GameDriver mooncrgx_driver;
extern struct GameDriver mooncrst_driver;
extern struct GameDriver mooncrsg_driver;
extern struct GameDriver mooncrsb_driver;
extern struct GameDriver fantazia_driver;
extern struct GameDriver eagle_driver;
extern struct GameDriver moonqsr_driver;
extern struct GameDriver checkman_driver;
extern struct GameDriver moonal2_driver;
extern struct GameDriver moonal2b_driver;
extern struct GameDriver kingball_driver;

/* "Scramble hardware" (and variations) games */
extern struct GameDriver scramble_driver;
extern struct GameDriver atlantis_driver;
extern struct GameDriver theend_driver;
extern struct GameDriver ckongs_driver;
extern struct GameDriver froggers_driver;
extern struct GameDriver amidars_driver;
extern struct GameDriver triplep_driver;
extern struct GameDriver scobra_driver;
extern struct GameDriver scobrak_driver;
extern struct GameDriver scobrab_driver;
extern struct GameDriver stratgyx_driver;
extern struct GameDriver stratgyb_driver;
extern struct GameDriver armorcar_driver;
extern struct GameDriver armorca2_driver;
extern struct GameDriver moonwar2_driver;
extern struct GameDriver monwar2a_driver;
extern struct GameDriver tazmania_driver;
extern struct GameDriver calipso_driver;
extern struct GameDriver anteater_driver;
extern struct GameDriver rescue_driver;
extern struct GameDriver minefld_driver;
extern struct GameDriver losttomb_driver;
extern struct GameDriver losttmbh_driver;
extern struct GameDriver superbon_driver;
extern struct GameDriver hustler_driver;
extern struct GameDriver billiard_driver;
extern struct GameDriver frogger_driver;
extern struct GameDriver frogsega_driver;
extern struct GameDriver frogger2_driver;
extern struct GameDriver amidar_driver;
extern struct GameDriver amidarjp_driver;
extern struct GameDriver amigo_driver;
extern struct GameDriver turtles_driver;
extern struct GameDriver turpin_driver;
extern struct GameDriver k600_driver;
extern struct GameDriver jumpbug_driver;
extern struct GameDriver jbugsega_driver;
extern struct GameDriver flyboy_driver;
extern struct GameDriver fastfred_driver;
extern struct GameDriver jumpcoas_driver;

/* "Crazy Climber hardware" games */
extern struct GameDriver cclimber_driver;
extern struct GameDriver ccjap_driver;
extern struct GameDriver ccboot_driver;
extern struct GameDriver ckong_driver;
extern struct GameDriver ckonga_driver;
extern struct GameDriver ckongjeu_driver;
extern struct GameDriver ckongalc_driver;
extern struct GameDriver monkeyd_driver;
extern struct GameDriver rpatrolb_driver;
extern struct GameDriver silvland_driver;
extern struct GameDriver swimmer_driver;
extern struct GameDriver swimmera_driver;
extern struct GameDriver guzzler_driver;

/* "Phoenix hardware" (and variations) games */
extern struct GameDriver phoenix_driver;
extern struct GameDriver phoenixt_driver;
extern struct GameDriver phoenix3_driver;
extern struct GameDriver phoenixc_driver;
extern struct GameDriver pleiads_driver;
extern struct GameDriver pleiadce_driver;
extern struct GameDriver naughtyb_driver;
extern struct GameDriver popflame_driver;

/* Nintendo games */
extern struct GameDriver radarscp_driver;
extern struct GameDriver dkong_driver;
extern struct GameDriver dkongjp_driver;
extern struct GameDriver dkongjr_driver;
extern struct GameDriver dkngjrjp_driver;
extern struct GameDriver dkjrjp_driver;
extern struct GameDriver dkjrbl_driver;
extern struct GameDriver dkong3_driver;
extern struct GameDriver mario_driver;
extern struct GameDriver masao_driver;
extern struct GameDriver hunchy_driver;
extern struct GameDriver herocast_driver;

/* Gremlin 8080 games */
extern struct GameDriver blockade_driver;
extern struct GameDriver comotion_driver;
extern struct GameDriver hustle_driver;
extern struct GameDriver blasto_driver;

/* Gremlin/Sega "VIC dual game board" games */
extern struct GameDriver depthch_driver;
extern struct GameDriver safari_driver;
extern struct GameDriver frogs_driver;
extern struct GameDriver sspaceat_driver;
extern struct GameDriver headon_driver;
extern struct GameDriver invho2_driver;
extern struct GameDriver samurai_driver;
extern struct GameDriver invinco_driver;
extern struct GameDriver invds_driver;
extern struct GameDriver tranqgun_driver;
extern struct GameDriver spacetrk_driver;
extern struct GameDriver sptrekct_driver;
extern struct GameDriver carnival_driver;
extern struct GameDriver pulsar_driver;
extern struct GameDriver heiankyo_driver;

/* Exidy games */
extern struct GameDriver sidetrac_driver;
extern struct GameDriver targ_driver;
extern struct GameDriver spectar_driver;
extern struct GameDriver spectar1_driver;
extern struct GameDriver venture_driver;
extern struct GameDriver venture2_driver;
extern struct GameDriver venture4_driver;
extern struct GameDriver mtrap_driver;
extern struct GameDriver pepper2_driver;
extern struct GameDriver hardhat_driver;
extern struct GameDriver fax_driver;
extern struct GameDriver circus_driver;
extern struct GameDriver robotbwl_driver;
extern struct GameDriver crash_driver;

/* SNK / Rock-ola games */
extern struct GameDriver sasuke_driver;
extern struct GameDriver satansat_driver;
extern struct GameDriver zarzon_driver;
extern struct GameDriver vanguard_driver;
extern struct GameDriver vangrdce_driver;
extern struct GameDriver fantasy_driver;
extern struct GameDriver pballoon_driver;
extern struct GameDriver nibbler_driver;
extern struct GameDriver nibblera_driver;

/* Stern "Berzerk hardware" games */
extern struct GameDriver berzerk_driver;
extern struct GameDriver berzerk1_driver;
extern struct GameDriver frenzy_driver;
extern struct GameDriver frenzy1_driver;

/* GamePlan games */
extern struct GameDriver megatack_driver;
extern struct GameDriver killcom_driver;
extern struct GameDriver challeng_driver;
extern struct GameDriver kaos_driver;

/* "stratovox hardware" games */
extern struct GameDriver route16_driver;
extern struct GameDriver stratvox_driver;
extern struct GameDriver speakres_driver;

/* Zaccaria games */
extern struct GameDriver monymony_driver;
extern struct GameDriver jackrabt_driver;
extern struct GameDriver jackrab2_driver;
extern struct GameDriver jackrabs_driver;

/* Cinematronics raster games */
extern struct GameDriver jack_driver;
extern struct GameDriver jacka_driver;
extern struct GameDriver zzyzzyxx_driver;
extern struct GameDriver brix_driver;
extern struct GameDriver sucasino_driver;

/* "The Pit hardware" games */
extern struct GameDriver roundup_driver;
extern struct GameDriver thepit_driver;
extern struct GameDriver intrepid_driver;
extern struct GameDriver portman_driver;
extern struct GameDriver suprmous_driver;

/* Valadon Automation games */
extern struct GameDriver bagman_driver;
extern struct GameDriver bagmans_driver;
extern struct GameDriver bagmans2_driver;
extern struct GameDriver sbagman_driver;
extern struct GameDriver sbagmans_driver;
extern struct GameDriver pickin_driver;

/* Seibu Denshi / Seibu Kaihatsu games */
extern struct GameDriver stinger_driver;
extern struct GameDriver scion_driver;
extern struct GameDriver scionc_driver;
extern struct GameDriver wiz_driver;

/* Universal games */
extern struct GameDriver cosmica_driver;
extern struct GameDriver cheekyms_driver;
extern struct GameDriver panic_driver;
extern struct GameDriver panica_driver;
extern struct GameDriver panicger_driver;
extern struct GameDriver ladybug_driver;
extern struct GameDriver ladybugb_driver;
extern struct GameDriver snapjack_driver;
extern struct GameDriver cavenger_driver;
extern struct GameDriver mrdo_driver;
extern struct GameDriver mrdot_driver;
extern struct GameDriver mrdofix_driver;
extern struct GameDriver mrlo_driver;
extern struct GameDriver mrdu_driver;
extern struct GameDriver mrdoy_driver;
extern struct GameDriver yankeedo_driver;
extern struct GameDriver docastle_driver;
extern struct GameDriver docastl2_driver;
extern struct GameDriver dounicorn_driver;
extern struct GameDriver dowild_driver;
extern struct GameDriver jjack_driver;
extern struct GameDriver dorunrun_driver;
extern struct GameDriver spiero_driver;
extern struct GameDriver kickridr_driver;

/* Midway 8080 b/w games */
extern struct GameDriver seawolf_driver;
extern struct GameDriver gunfight_driver;
extern struct GameDriver tornbase_driver;
extern struct GameDriver zzzap_driver;
extern struct GameDriver maze_driver;
extern struct GameDriver boothill_driver;
extern struct GameDriver checkmat_driver;
extern struct GameDriver gmissile_driver;
extern struct GameDriver clowns_driver;
extern struct GameDriver spcenctr_driver;
extern struct GameDriver invaders_driver;
extern struct GameDriver invdelux_driver;
extern struct GameDriver invadpt2_driver;
extern struct GameDriver earthinv_driver;
extern struct GameDriver spaceatt_driver;
extern struct GameDriver sinvemag_driver;
extern struct GameDriver invrvnge_driver;
extern struct GameDriver invrvnga_driver;
extern struct GameDriver galxwars_driver;
extern struct GameDriver lrescue_driver;
extern struct GameDriver grescue_driver;
extern struct GameDriver desterth_driver;
extern struct GameDriver cosmicmo_driver;
extern struct GameDriver spaceph_driver;
extern struct GameDriver rollingc_driver;
extern struct GameDriver bandido_driver;
extern struct GameDriver ozmawars_driver;
extern struct GameDriver schaser_driver;
extern struct GameDriver lupin3_driver;
extern struct GameDriver helifire_driver;
extern struct GameDriver helifira_driver;
extern struct GameDriver spacefev_driver;
extern struct GameDriver sfeverbw_driver;
extern struct GameDriver astlaser_driver;
extern struct GameDriver intruder_driver;
extern struct GameDriver polaris_driver;
extern struct GameDriver polarisa_driver;
extern struct GameDriver lagunar_driver;
extern struct GameDriver m4_driver;
extern struct GameDriver phantom2_driver;
extern struct GameDriver dogpatch_driver;
extern struct GameDriver midwbowl_driver;
extern struct GameDriver blueshrk_driver;
extern struct GameDriver einnings_driver;
extern struct GameDriver dplay_driver;
extern struct GameDriver m79amb_driver;

/* "Midway" Z80 b/w games */
extern struct GameDriver astinvad_driver;
extern struct GameDriver kamikaze_driver;

/* Meadows S2650 games */
extern struct GameDriver lazercmd_driver;
extern struct GameDriver deadeye_driver;
extern struct GameDriver gypsyjug_driver;
extern struct GameDriver medlanes_driver;

/* Midway "Astrocade" games */
extern struct GameDriver wow_driver;
extern struct GameDriver robby_driver;
extern struct GameDriver gorf_driver;
extern struct GameDriver gorfpgm1_driver;
extern struct GameDriver seawolf2_driver;
extern struct GameDriver spacezap_driver;

const struct GameDriver *drivers[] =
{
	&amidar_driver,		/* GX337 (c) 1982 Konami + Stern license */
	&amidarjp_driver,	/* GX337 (c) 1981 Konami */
	&amidars_driver,	/* (c) 1982 Konami */
	&amigo_driver,		/* bootleg */
	&anteater_driver,	/* (c) 1982 Tago */
	&armorca2_driver,	/* (c) 1981 Stern */
	&armorcar_driver,	/* (c) 1981 Stern */
	&astinvad_driver,	/* (c) 1980 Stern */
	&astlaser_driver,
	&atlantis_driver,	/* (c) 1981 Comsoft */
	&bagman_driver,		/* (c) 1982 */
	&bagmans2_driver,	/* (c) 1982 + Stern license */
	&bagmans_driver,	/* (c) 1982 + Stern license */
	&bandido_driver,	/* (c) Exidy */
	&beastf_driver,		/* (c) 1984 Epos Corporation */
	&berzerk1_driver,	/* (c) 1980 */
	&berzerk_driver,	/* (c) 1980 */
	&billiard_driver,
	&blasto_driver,		/* [1978 Gremlin] */
	&blockade_driver,	/* 1-4 [1977 Gremlin] */
	&blueshrk_driver,	/* 742 [1978] */
	&boothill_driver,	/* 612 [1977] */
	&brix_driver,		/* (c) 1982 Cinematronics */
	&calipso_driver,	/* (c) 1982 Tago */
	&carnival_driver,	/* 651-666 (c) 1980 Sega */
	&cavenger_driver,	/* (c) 1981 */
	&ccboot_driver,		/* bootleg */
	&ccjap_driver,		/* (c) 1980 Nichibutsu */
	&cclimber_driver,	/* (c) 1980 Nichibutsu */
	&challeng_driver,	/* (c) 1981 Centuri */
	&checkman_driver,	/* (c) 1982 Zilec-Zenitone */
	&checkmat_driver,	/* 615 [1977] */
	&cheekyms_driver,	/* (c) [1980?] */
	&circus_driver,		/* no copyright notice [1977?] */
	&ckong_driver,		/* (c) 1981 Falcon */
	&ckonga_driver,		/* (c) 1981 Falcon */
	&ckongalc_driver,	/* bootleg */
	&ckongjeu_driver,	/* bootleg */
	&ckongs_driver,		/* bootleg */
	&clowns_driver,		/* 630 [1978] */
	&comotion_driver,	/* 5-7 [1977 Gremlin] */
	&cosmica_driver,	/* (c) [1979] */
	&cosmicmo_driver,	/* Universal */
	&crash_driver,		/* Exidy [1979?] */
	&crush_driver,		/* bootleg Make Trax */
	&deadeye_driver,	/* [1978?] */
	&depthch_driver,	/* 50-55 [1977 Gremlin?] */
	&desterth_driver,	/* bootleg */
	&dkjrbl_driver,		/* (c) 1982 Nintendo of America */
	&dkjrjp_driver,		/* (c) 1982 Nintendo */
	&dkngjrjp_driver,	/* no copyright notice */
	&dkong3_driver,		/* (c) 1983 Nintendo of America */
	&dkong_driver,		/* (c) 1981 Nintendo of America */
	&dkongjp_driver,	/* (c) 1981 Nintendo */
	&dkongjr_driver,	/* (c) 1982 Nintendo of America */
	&docastl2_driver,	/* (c) 1983 */
	&docastle_driver,	/* (c) 1983 */
	&dogpatch_driver,	/* 644 [1977] */
	&dorunrun_driver,	/* (c) 1984 */
	&dounicorn_driver,	/* (c) 1983 */
	&dowild_driver,		/* (c) 1984 */
	&dplay_driver,		/* 619 [1977] */
	&eagle_driver,		/* (c) Centuri */
	&earthinv_driver,
	&einnings_driver,	/* 642 [1978] Midway */
	&eyes_driver,		/* (c) 1982 Digitrex Techstar + "Rockola presents" */
	&fantasy_driver,	/* (c) 1981 Rock-ola */
	&fantazia_driver,	/* bootleg */
	&fastfred_driver,	/* (c) 1982 Atari */
	&fax_driver,		/* (c) 1983 */
	&flyboy_driver,		/* (c) 1982 Kaneko (bootleg?) */
	&frenzy1_driver,	/* (c) 1982 */
	&frenzy_driver,		/* (c) 1982 */
	&frogger2_driver,	/* 800-3110 (c) 1981 Sega */
	&frogger_driver,	/* 834-0068 (c) 1981 Sega */
	&froggers_driver,	/* bootleg */
	&frogs_driver,		/* 112-119 [1978 Gremlin?] */
	&frogsega_driver,	/* (c) 1981 Sega */
	&galap1_driver,		/* hack */
	&galap4_driver,		/* hack */
	&galapx_driver,		/* hack */
	&galaxian_driver,	/* (c) Namco */
	&galmidw_driver,	/* (c) Midway */
	&galnamco_driver,	/* hack */
	&galturbo_driver,	/* hack */
	&galxwars_driver,
	&gmissile_driver,	/* 623 [1977] */
	&gorf_driver,		/* (c) 1981 */
	&gorfpgm1_driver,	/* (c) 1981 */
	&grescue_driver,	/* bootleg? */
	&gunfight_driver,	/* 597 [1975] */
	&guzzler_driver,	/* (c) 1983 Tehkan */
	&gypsyjug_driver,	/* [1978?] */
	&hangly_driver,		/* hack */
	&hardhat_driver,	/* (c) 1982 */
	&headon_driver,		/* 163-167/192-193 (c) Gremlin */
	&heiankyo_driver,	/* (c) [1979?] Denki Onkyo */
	&helifira_driver,	/* (c) Nintendo */
	&helifire_driver,	/* (c) Nintendo */
	&hustle_driver,		/* 16-21 [1977 Gremlin] */
	&hustler_driver,	/* GX343 (c) 1981 Konami */
	&intrepid_driver,	/* (c) 1983 Nova Games Ltd. */
	&intruder_driver,
	&invaders_driver,	/* 739 [1979] */
	&invadpt2_driver,	/* 851 [1980] Taito */
	&invdelux_driver,	/* 852 [1980] Midway */
	&invds_driver,		/* 367-382 (c) 1979 Sega */
	&invho2_driver,		/* 271-286 (c) 1979 Sega */
	&invinco_driver,	/* 310-318 (c) 1979 Sega */
	&invrvnga_driver,
	&invrvnge_driver,
	&jack_driver,		/* (c) 1982 Cinematronics */
	&jacka_driver,		/* (c) 1982 Cinematronics */
	&jackrab2_driver,	/* (c) 1984 */
	&jackrabs_driver,	/* (c) 1984 */
	&jackrabt_driver,	/* (c) 1984 */
	&japirem_driver,	/* (c) Irem */
	&jbugsega_driver,	/* (c) 1981 Sega */
	&jjack_driver,		/* (c) 1984 */
	&jrpacman_driver,	/* (c) 1983 Midway */
	&jumpbug_driver,	/* (c) 1981 Rock-ola */
	&jumpcoas_driver,	/* (c) 1983 Kaneko */
	&k600_driver,		/* GX353 (c) 1981 Konami */
	&kamikaze_driver,	/* Leijac Corporation */
	&kaos_driver,		/* (c) 1981 */
	&kickridr_driver,	/* (c) 1984 */
	&killcom_driver,	/* (c) 1980 Centuri */
	&kingball_driver,	/* no copyright notice (the char set contains Namco) */
	&ladybug_driver,	/* (c) 1981 */
	&ladybugb_driver,	/* bootleg */
	&lagunar_driver,	/* 622 [1977] */
	&lazercmd_driver,	/* [1976?] */
	&lizwiz_driver,		/* (c) 1985 Techstar + "Sunn presents" */
	&losttmbh_driver,	/* (c) 1982 Stern */
	&losttomb_driver,	/* (c) 1982 Stern */
	&lrescue_driver,	/* (c) 1979 Taito */
	&lupin3_driver,		/* (c) 1980 Taito */
	&m4_driver,			/* 626 [1977] */
	&m79amb_driver,
	&maketrax_driver,	/* (c) 1981 Williams */
	&mario_driver,		/* (c) 1983 Nintendo of America */
	&masao_driver,		/* bootleg */
	&maze_driver,		/* 611 [1976] */
	&medlanes_driver,	/* [1977?] */
	&megatack_driver,	/* (c) 1980 Centuri */
	&midwbowl_driver,	/* 730 [1978] Midway */
	&minefld_driver,	/* (c) 1983 Stern */
	&monkeyd_driver,	/* bootleg */
	&monwar2a_driver,	/* (c) 1981 Stern */
	&monymony_driver,	/* (c) 1983 */
	&moonal2_driver,	/* Nichibutsu */
	&moonal2b_driver,	/* Nichibutsu */
	&mooncrgx_driver,	/* bootleg */
	&mooncrsb_driver,	/* bootleg */
	&mooncrsg_driver,	/* (c) 1980 Gremlin */
	&mooncrst_driver,	/* (c) 1980 Nichibutsu */
	&moonqsr_driver,	/* (c) 1980 Nichibutsu */
	&moonwar2_driver,	/* (c) 1981 Stern */
	&mrdo_driver,		/* (c) 1982 */
	&mrdofix_driver,	/* (c) 1982 + Taito license */
	&mrdot_driver,		/* (c) 1982 + Taito license */
	&mrdoy_driver,		/* bootleg */
	&mrdu_driver,		/* bootleg */
	&mrlo_driver,		/* bootleg */
	&mrtnt_driver,		/* (c) 1983 Telko */
	&mspacatk_driver,	/* hack */
	&mspacman_driver,	/* (c) 1981 Midway (but it's a bootleg) */
	&mtrap_driver,		/* (c) 1981 */
	&namcopac_driver,	/* (c) 1980 Namco */
	&naughtyb_driver,	/* (c) 1982 Jaleco + Cinematronics */
	&nibbler_driver,	/* (c) 1982 Rock-ola */
	&nibblera_driver,	/* (c) 1982 Rock-ola */
	&ozmawars_driver,	/* Shin Nihon Kikaku (SNK) */
	&pacman_driver,		/* (c) 1980 Midway */
	&pacmanbl_driver,	/* bootleg */
	&pacmanjp_driver,	/* (c) 1980 Namco */
	&pacmod_driver,		/* (c) 1981 Midway */
	&pacplus_driver,
	&panic_driver,		/* (c) 1980 */
	&panica_driver,		/* (c) 1980 */
	&panicger_driver,	/* (c) 1980 */
	&pballoon_driver,	/* (c) 1982 SNK */
	&pengo2_driver,		/* 834-0386 (c) 1982 Sega */
	&pengo2u_driver,	/* 834-0386 (c) 1982 Sega */
	&pengo_driver,		/* 834-0386 (c) 1982 Sega */
	&penta_driver,		/* bootleg */
	&pepper2_driver,	/* (c) 1982 */
	&phantom2_driver,	/* 652 [1979] */
	&phoenix3_driver,	/* bootleg */
	&phoenix_driver,	/* (c) 1980 Amstar */
	&phoenixc_driver,	/* bootleg */
	&phoenixt_driver,	/* (c) 1980 Taito */
	&pickin_driver,		/* (c) 1983 */
	&piranha_driver,	/* hack */
	&pisces_driver,		/* ? */
	&pleiadce_driver,	/* (c) 1981 Centuri + Tehkan */
	&pleiads_driver,	/* (c) 1981 Tehkan */
	&polaris_driver,	/* (c) 1980 Taito */
	&polarisa_driver,	/* (c) 1980 Taito */
	&ponpoko_driver,	/* (c) 1982 Sigma Ent. Inc. */
	&popflame_driver,	/* (c) 1982 Jaleco */
	&portman_driver,	/* (c) 1982 Nova Games Ltd. */
	&puckman_driver,	/* hack */
	&pulsar_driver,		/* 790-805 (c) 1981 Sega */
	&radarscp_driver,	/* (c) 1980 Nintendo */
	&redufo_driver,		/* ? */
	&rescue_driver,		/* (c) 1982 Stern */
	&robby_driver,		/* (c) 1981 */
	&robotbwl_driver,	/* no copyright notice */
	&rollingc_driver,	/* Nichibutsu */
	&roundup_driver,	/* (c) 1981 Amenip/Centuri */
	&route16_driver,	/* (c) 1981 Leisure and Allied (bootleg) */
	&rpatrolb_driver,	/* bootleg */
	&safari_driver,		/* 57-66 [1977 Gremlin?] */
	&samurai_driver,	/* 289-302 + upgrades (c) 1980 Sega */
	&sasuke_driver,		/* [1980] Shin Nihon Kikaku (SNK) */
	&satansat_driver,	/* (c) 1981 SNK */
	&sbagman_driver,	/* (c) 1984 */
	&sbagmans_driver,	/* (c) 1984 + Stern license */
	&schaser_driver,	/* Taito */
	&scion_driver,		/* (c) 1984 Seibu Denshi */
	&scionc_driver,		/* (c) 1984 Seibu Denshi + Cinematronics license */
	&scobra_driver,		/* GX316 (c) 1981 Stern */
	&scobrab_driver,	/* GX316 (c) 1981 Karateco (bootleg?) */
	&scobrak_driver,	/* GX316 (c) 1981 Konami */
	&scramble_driver,	/* GX387 (c) 1981 Stern */
	&seawolf2_driver,
	&seawolf_driver,	/* 596 [1976] */
	&sfeverbw_driver,
	&sidetrac_driver,	/* (c) 1979 */
	&silvland_driver,	/* Falcon */
	&sinvemag_driver,
	&snapjack_driver,	/* (c) */
	&spaceatt_driver,
	&spacefev_driver,
	&spaceph_driver,	/* Zilec Games */
	&spacetrk_driver,	/* 630-645 (c) 1980 Sega */
	&spacezap_driver,	/* (c) 1980 */
	&spcenctr_driver,	/* 645 (c) 1980 Midway */
	&speakres_driver,	/* no copyright notice */
	&spectar1_driver,	/* (c) 1980 */
	&spectar_driver,	/* (c) 1980 */
	&spiero_driver,		/* (c) 1987 */
	&sptrekct_driver,	/* (c) 1980 Sega */
	&sspaceat_driver,	/* 139-146 (c) */
	&stinger_driver,	/* (c) 1983 Seibu Denshi */
	&stratgyb_driver,	/* bootleg (of the Konami version?) */
	&stratgyx_driver,	/* (c) 1981 Stern */
	&stratvox_driver,	/* Taito */
	&sucasino_driver,	/* (c) 1982 Data Amusement */
	&superbon_driver,	/* bootleg */
	&superg_driver,		/* hack */
	&suprmous_driver,	/* (c) 1982 Taito */
	&swimmer_driver,	/* (c) 1982 Tehkan */
	&swimmera_driver,	/* (c) 1982 Tehkan */
	&targ_driver,		/* (c) 1980 */
	&tazmania_driver,	/* (c) 1982 Stern */
	&theend_driver,		/* (c) 1980 Stern */
	&theglob_driver,	/* (c) 1983 Epos Corporation */
	&thepit_driver,		/* (c) 1982 Centuri */
	&tornbase_driver,	/* 605 [1976] */
	&tranqgun_driver,	/* 413-428 (c) 1980 Sega */
	&triplep_driver,	/* (c) 1982 KKI */
	&turpin_driver,		/* (c) 1981 Sega */
	&turtles_driver,	/* (c) 1981 Stern */
	&uniwars_driver,	/* (c) Karateco (bootleg?) */
	&vangrdce_driver,	/* (c) 1981 SNK + Centuri */
	&vanguard_driver,	/* (c) 1981 SNK */
	&venture2_driver,	/* (c) 1981 */
	&venture4_driver,	/* (c) 1981 */
	&venture_driver,	/* (c) 1981 */
	&warofbug_driver,	/* (c) 1981 Armenia */
	&wiz_driver,		/* (c) 1985 Seibu Kaihatsu */
	&wow_driver,		/* (c) 1980 */
	&yankeedo_driver,	/* bootleg */
	&zarzon_driver,		/* (c) 1981 Taito, gameplay says SNK */
	&zigzag2_driver,	/* (c) 1982 LAX */
	&zigzag_driver,		/* (c) 1982 LAX */
	&zzyzzyxx_driver,	/* (c) 1982 Cinematronics */
	&zzzap_driver,		/* 610 [1976] */

	0	/* end of array */
};
