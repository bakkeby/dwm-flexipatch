/* Bar functionality */
#include "bar_indicators.c"
#include "bar_tagicons.c"
#include "bar.c"

#if BAR_ALPHA_PATCH
#include "bar_alpha.c"
#endif
#if BAR_ALTERNATIVE_TAGS_PATCH
#include "bar_alternativetags.c"
#endif
#if BAR_ANYBAR_PATCH
#include "bar_anybar.c"
#endif
#if BAR_DWMBLOCKS_PATCH && BAR_STATUSCMD_PATCH
#include "bar_dwmblocks.c"
#endif
#if BAR_EWMHTAGS_PATCH
#include "bar_ewmhtags.c"
#endif
#if COMBO_PATCH
#include "combo.c"
#endif
#if BAR_LAUNCHER_PATCH
#include "bar_launcher.c"
#endif
#if BAR_LTSYMBOL_PATCH
#include "bar_ltsymbol.c"
#endif
#if BAR_POWERLINE_STATUS_PATCH
#include "bar_powerline_status.c"
#endif
#if BAR_POWERLINE_TAGS_PATCH
#include "bar_powerline_tags.c"
#endif
#if BAR_STATUS_PATCH
#include "bar_status.c"
#endif
#if BAR_STATUS2D_PATCH
#include "bar_status2d.c"
#endif
#if BAR_STATUSBUTTON_PATCH
#include "bar_statusbutton.c"
#endif
#if BAR_STATUSCMD_PATCH
#include "bar_statuscmd.c"
#endif
#if BAR_STATUSCOLORS_PATCH
#include "bar_statuscolors.c"
#endif
#if BAR_WINICON_PATCH
#include "bar_winicon.c"
#endif
#if BAR_TABGROUPS_PATCH
#include "bar_tabgroups.c"
#endif
#if BAR_TAGPREVIEW_PATCH
#include "bar_tagpreview.c"
#endif
#if BAR_TAGS_PATCH
#include "bar_tags.c"
#endif
#if BAR_TAGLABELS_PATCH
#include "bar_taglabels.c"
#endif
#if BAR_TAGGRID_PATCH
#include "bar_taggrid.c"
#endif
#if BAR_WINTITLE_PATCH
#include "bar_wintitle.c"
#endif
#if BAR_FANCYBAR_PATCH
#include "bar_fancybar.c"
#endif
#if BAR_FLEXWINTITLE_PATCH
#include "bar_flexwintitle.c"
#if BAR_WINTITLE_FLOATING_PATCH
#include "bar_wintitle_floating.c"
#endif
#if BAR_WINTITLE_HIDDEN_PATCH
#include "bar_wintitle_hidden.c"
#endif
#endif // BAR_FLEXWINTITLE_PATCH
#if BAR_AWESOMEBAR_PATCH
#include "bar_awesomebar.c"
#endif
#if BAR_SYSTRAY_PATCH
#include "bar_systray.c"
#endif
#if BAR_HOLDBAR_PATCH
#include "bar_holdbar.c"
#endif
#if BAR_VTCOLORS_PATCH
#include "bar_vtcolors.c"
#endif
#if BAR_WINTITLEACTIONS_PATCH
#include "bar_wintitleactions.c"
#endif
#if BAR_LAYOUTMENU_PATCH
#include "bar_layoutmenu.c"
#endif

/* Other patches */
#if ALT_TAB_PATCH
#include "alttab.c"
#endif
#if ASPECTRESIZE_PATCH
#include "aspectresize.c"
#endif
#if ATTACHABOVE_PATCH || ATTACHASIDE_PATCH || ATTACHBELOW_PATCH || ATTACHBOTTOM_PATCH || SEAMLESS_RESTART_PATCH
#include "attachx.c"
#endif
#if AUTOSTART_PATCH
#include "autostart.c"
#endif
#if CFACTS_PATCH
#include "cfacts.c"
#endif
#if CMDCUSTOMIZE_PATCH
#include "cmdcustomize.c"
#endif
#if COOL_AUTOSTART_PATCH
#include "cool_autostart.c"
#endif
#if CYCLELAYOUTS_PATCH
#include "cyclelayouts.c"
#endif
#if DECORATION_HINTS_PATCH
#include "decorationhints.c"
#endif
#if DISTRIBUTETAGS_PATCH
#include "distributetags.c"
#endif
#if DRAGCFACT_PATCH && CFACTS_PATCH
#include "dragcfact.c"
#endif
#if DWMC_PATCH
#include "dwmc.c"
#elif FSIGNAL_PATCH
#include "fsignal.c"
#endif
#if EXRESIZE_PATCH
#include "exresize.c"
#endif
#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
#include "fakefullscreenclient.c"
#endif
#if FLOATPOS_PATCH
#include "floatpos.c"
#endif
#if FOCUSADJACENTTAG_PATCH
#include "focusadjacenttag.c"
#endif
#if FOCUSDIR_PATCH
#include "focusdir.c"
#endif
#if FOCUSFOLLOWMOUSE_PATCH
#include "focusfollowmouse.c"
#endif
#if FOCUSMASTER_PATCH || FOCUSMASTER_RETURN_PATCH
#include "focusmaster.c"
#endif
#if FOCUSURGENT_PATCH
#include "focusurgent.c"
#endif
#if FULLSCREEN_PATCH
#include "fullscreen.c"
#endif
#if INPLACEROTATE_PATCH
#include "inplacerotate.c"
#endif
#if IPC_PATCH
#include "ipc.c"
#ifdef VERSION
#include "ipc/IPCClient.c"
#include "ipc/yajl_dumps.c"
#include "ipc/ipc.c"
#include "ipc/util.c"
#endif
#endif // IPC_PATCH
#if INSETS_PATCH
#include "insets.c"
#endif
#if KEYMODES_PATCH
#include "keymodes.c"
#endif
#if KILLUNSEL_PATCH
#include "killunsel.c"
#endif
#if MAXIMIZE_PATCH
#include "maximize.c"
#endif
#if MPDCONTROL_PATCH
#include "mpdcontrol.c"
#endif
#if MOVEPLACE_PATCH
#include "moveplace.c"
#endif
#if MOVERESIZE_PATCH
#include "moveresize.c"
#endif
#if MOVESTACK_PATCH
#include "movestack.c"
#endif
#if NAMETAG_PATCH
#include "nametag.c"
#endif
#if NO_MOD_BUTTONS_PATCH
#include "nomodbuttons.c"
#endif
#if PERTAG_PATCH
#include "pertag.c"
#endif
#if PLACEDIR_PATCH
#include "placedir.c"
#endif
#if PLACEMOUSE_PATCH
#include "placemouse.c"
#endif
#if PUSH_NO_MASTER_PATCH
#include "push_no_master.c"
#elif PUSH_PATCH
#include "push.c"
#endif
#if REORGANIZETAGS_PATCH
#include "reorganizetags.c"
#endif
#if RESTARTSIG_PATCH
#include "restartsig.c"
#endif
#if RIODRAW_PATCH
#include "riodraw.c"
#endif
#if ROTATESTACK_PATCH
#include "rotatestack.c"
#endif
#if ROUNDED_CORNERS_PATCH
#include "roundedcorners.c"
#endif
#if RENAMED_SCRATCHPADS_PATCH
#include "renamed_scratchpads.c"
#elif SCRATCHPADS_PATCH
#include "scratchpad.c"
#endif
#if SCRATCHPAD_ALT_1_PATCH
#include "scratchpad_alt_1.c"
#endif
#if SELFRESTART_PATCH
#include "selfrestart.c"
#endif
#if SETBORDERPX_PATCH
#include "setborderpx.c"
#endif
#if SHIFTBOTH_PATCH || SHIFTSWAPTAGS_PATCH || SHIFTTAG_PATCH || SHIFTTAGCLIENTS_PATCH || SHIFTVIEW_PATCH || SHIFTVIEW_CLIENTS_PATCH
#include "shift.c"
#endif
#if SHIFTBOTH_PATCH
#include "shiftboth.c"
#endif
#if SHIFTSWAPTAGS_PATCH && SWAPTAGS_PATCH
#include "shiftswaptags.c"
#endif
#if SHIFTTAG_PATCH
#include "shifttag.c"
#endif
#if SHIFTTAGCLIENTS_PATCH
#include "shifttagclients.c"
#endif
#if SHIFTVIEW_PATCH
#include "shiftview.c"
#endif
#if SHIFTVIEW_CLIENTS_PATCH
#include "shiftviewclients.c"
#endif
#if SIZEHINTS_RULED_PATCH
#include "sizehints_ruled.c"
#endif
#if SORTSCREENS_PATCH
#ifdef XINERAMA
#include "sortscreens.c"
#endif // XINERAMA
#endif
#if STACKER_PATCH
#include "stacker.c"
#endif
#if STICKY_PATCH
#include "sticky.c"
#endif
#if SWALLOW_PATCH
#include "swallow.c"
#endif
#if SWAPFOCUS_PATCH && PERTAG_PATCH
#include "swapfocus.c"
#endif
#if SWAPTAGS_PATCH
#include "swaptags.c"
#endif
#if SWITCHCOL_PATCH
#include "switchcol.c"
#endif
#if TAB_PATCH
#include "tab.c"
#endif
#if TAGALL_PATCH
#include "tagall.c"
#endif
#if TAGALLMON_PATCH
#include "tagallmon.c"
#endif
#if TAGOTHERMONITOR_PATCH
#include "tagothermonitor.c"
#endif
#if TAGSWAPMON_PATCH
#include "tagswapmon.c"
#endif
#if TAPRESIZE_PATCH
#include "tapresize.c"
#endif
#if TOGGLEFULLSCREEN_PATCH
#include "togglefullscreen.c"
#endif
#if TOGGLETOPBAR_PATCH
#include "toggletopbar.c"
#endif
#if TRANSFER_PATCH
#include "transfer.c"
#endif
#if TRANSFER_ALL_PATCH
#include "transferall.c"
#endif
#if UNFLOATVISIBLE_PATCH
#include "unfloatvisible.c"
#endif
#if VANITYGAPS_PATCH
#include "vanitygaps.c"
#endif
#if WARP_PATCH
#include "warp.c"
#endif
#if WINVIEW_PATCH
#include "winview.c"
#endif
#if ZOOMSWAP_PATCH
#include "zoomswap.c"
#endif
#if XKB_PATCH
#include "xkb.c"
#endif
#if XRDB_PATCH && !BAR_VTCOLORS_PATCH
#include "xrdb.c"
#endif
#if DRAGMFACT_PATCH
#include "dragmfact.c"
#endif
#if SEAMLESS_RESTART_PATCH
#include "seamless_restart.c"
#endif
/* Layouts */
#if BSTACK_LAYOUT || BSTACKHORIZ_LAYOUT || CENTEREDMASTER_LAYOUT || CENTEREDFLOATINGMASTER_LAYOUT || COLUMNS_LAYOUT || DECK_LAYOUT || TILE_LAYOUT
#include "layout_facts.c"
#endif
#if BSTACK_LAYOUT
#include "layout_bstack.c"
#endif
#if BSTACKHORIZ_LAYOUT
#include "layout_bstackhoriz.c"
#endif
#if CENTEREDMASTER_LAYOUT
#include "layout_centeredmaster.c"
#endif
#if CENTEREDFLOATINGMASTER_LAYOUT
#include "layout_centeredfloatingmaster.c"
#endif
#if COLUMNS_LAYOUT
#include "layout_columns.c"
#endif
#if DECK_LAYOUT
#include "layout_deck.c"
#endif
#if FIBONACCI_DWINDLE_LAYOUT || FIBONACCI_SPIRAL_LAYOUT
#include "layout_fibonacci.c"
#endif
#if FLEXTILE_DELUXE_LAYOUT
#include "layout_flextile-deluxe.c"
#endif
#if GAPPLESSGRID_LAYOUT
#include "layout_gapplessgrid.c"
#endif
#if GRIDMODE_LAYOUT
#include "layout_grid.c"
#endif
#if HORIZGRID_LAYOUT
#include "layout_horizgrid.c"
#endif
#if MONOCLE_LAYOUT
#include "layout_monocle.c"
#endif
#if NROWGRID_LAYOUT
#include "layout_nrowgrid.c"
#endif
#if TILE_LAYOUT
#include "layout_tile.c"
#endif

