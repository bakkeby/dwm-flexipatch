/* Bar functionality */
#include "bar_indicators.h"
#include "bar_tagicons.h"
#include "bar.h"

#if BAR_ALPHA_PATCH
#include "bar_alpha.h"
#endif
#if BAR_ALTERNATIVE_TAGS_PATCH
#include "bar_alternativetags.h"
#endif
#if BAR_ANYBAR_PATCH
#include "bar_anybar.h"
#endif
#if BAR_DWMBLOCKS_PATCH && BAR_STATUSCMD_PATCH
#include "bar_dwmblocks.h"
#endif
#if BAR_EWMHTAGS_PATCH
#include "bar_ewmhtags.h"
#endif
#if COMBO_PATCH
#include "combo.h"
#endif
#if BAR_HOLDBAR_PATCH
#include "bar_holdbar.h"
#endif
#if BAR_LAUNCHER_PATCH
#include "bar_launcher.h"
#endif
#if BAR_LTSYMBOL_PATCH
#include "bar_ltsymbol.h"
#endif
#if BAR_POWERLINE_STATUS_PATCH
#include "bar_powerline_status.h"
#endif
#if BAR_POWERLINE_TAGS_PATCH
#include "bar_powerline_tags.h"
#endif
#if BAR_STATUS_PATCH
#include "bar_status.h"
#endif
#if BAR_STATUS2D_PATCH
#include "bar_status2d.h"
#endif
#if BAR_STATUSBUTTON_PATCH
#include "bar_statusbutton.h"
#endif
#if BAR_STATUSCMD_PATCH
#include "bar_statuscmd.h"
#endif
#if BAR_STATUSCOLORS_PATCH
#include "bar_statuscolors.h"
#endif
#if BAR_WINICON_PATCH
#include "bar_winicon.h"
#endif
#if BAR_TABGROUPS_PATCH
#include "bar_tabgroups.h"
#endif
#if BAR_TAGS_PATCH
#include "bar_tags.h"
#endif
#if BAR_TAGLABELS_PATCH
#include "bar_taglabels.h"
#endif
#if BAR_TAGPREVIEW_PATCH
#include "bar_tagpreview.h"
#endif
#if BAR_TAGGRID_PATCH
#include "bar_taggrid.h"
#endif
#if BAR_WINTITLE_PATCH
#include "bar_wintitle.h"
#endif
#if BAR_FANCYBAR_PATCH
#include "bar_fancybar.h"
#endif
#if BAR_FLEXWINTITLE_PATCH
#include "bar_flexwintitle.h"
#if BAR_WINTITLE_FLOATING_PATCH
#include "bar_wintitle_floating.h"
#endif
#if BAR_WINTITLE_HIDDEN_PATCH
#include "bar_wintitle_hidden.h"
#endif
#endif // BAR_FLEXWINTITLE_PATCH
#if BAR_AWESOMEBAR_PATCH
#include "bar_awesomebar.h"
#endif
#if BAR_SYSTRAY_PATCH
#include "bar_systray.h"
#endif
#if BAR_VTCOLORS_PATCH
#include "bar_vtcolors.h"
#endif
#if BAR_WINTITLEACTIONS_PATCH
#include "bar_wintitleactions.h"
#endif
#if BAR_LAYOUTMENU_PATCH
#include "bar_layoutmenu.h"
#endif

/* Other patches */
#if ALT_TAB_PATCH
#include "alttab.h"
#endif
#if ASPECTRESIZE_PATCH
#include "aspectresize.h"
#endif
#if ATTACHABOVE_PATCH || ATTACHASIDE_PATCH || ATTACHBELOW_PATCH || ATTACHBOTTOM_PATCH || SEAMLESS_RESTART_PATCH
#include "attachx.h"
#endif
#if AUTOSTART_PATCH
#include "autostart.h"
#endif
#if CFACTS_PATCH
#include "cfacts.h"
#endif
#if CMDCUSTOMIZE_PATCH
#include "cmdcustomize.h"
#endif
#if COOL_AUTOSTART_PATCH
#include "cool_autostart.h"
#endif
#if CYCLELAYOUTS_PATCH
#include "cyclelayouts.h"
#endif
#if DECORATION_HINTS_PATCH
#include "decorationhints.h"
#endif
#if DISTRIBUTETAGS_PATCH
#include "distributetags.h"
#endif
#if DRAGCFACT_PATCH && CFACTS_PATCH
#include "dragcfact.h"
#endif
#if DRAGMFACT_PATCH
#include "dragmfact.h"
#endif
#if DWMC_PATCH
#include "dwmc.h"
#elif FSIGNAL_PATCH
#include "fsignal.h"
#endif
#if EXRESIZE_PATCH
#include "exresize.h"
#endif
#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
#include "fakefullscreenclient.h"
#endif
#if FLOATPOS_PATCH
#include "floatpos.h"
#endif
#if FOCUSDIR_PATCH
#include "focusdir.h"
#endif
#if FOCUSADJACENTTAG_PATCH
#include "focusadjacenttag.h"
#endif
#if FOCUSFOLLOWMOUSE_PATCH
#include "focusfollowmouse.h"
#endif
#if FOCUSMASTER_PATCH || FOCUSMASTER_RETURN_PATCH
#include "focusmaster.h"
#endif
#if FOCUSURGENT_PATCH
#include "focusurgent.h"
#endif
#if FULLSCREEN_PATCH
#include "fullscreen.h"
#endif
#if INPLACEROTATE_PATCH
#include "inplacerotate.h"
#endif
#if IPC_PATCH
#include "ipc.h"
#include "ipc/ipc.h"
#include "ipc/util.h"
#endif
#if INSETS_PATCH
#include "insets.h"
#endif
#if KEYMODES_PATCH
#include "keymodes.h"
#endif
#if KILLUNSEL_PATCH
#include "killunsel.h"
#endif
#if MAXIMIZE_PATCH
#include "maximize.h"
#endif
#if MPDCONTROL_PATCH
#include "mpdcontrol.h"
#endif
#if MOVEPLACE_PATCH
#include "moveplace.h"
#endif
#if MOVERESIZE_PATCH
#include "moveresize.h"
#endif
#if MOVESTACK_PATCH
#include "movestack.h"
#endif
#if NAMETAG_PATCH
#include "nametag.h"
#endif
#if NO_MOD_BUTTONS_PATCH
#include "nomodbuttons.h"
#endif
#if PERTAG_PATCH
#include "pertag.h"
#endif
#if PLACEDIR_PATCH
#include "placedir.h"
#endif
#if PLACEMOUSE_PATCH
#include "placemouse.h"
#endif
#if PUSH_NO_MASTER_PATCH
#include "push_no_master.h"
#elif PUSH_PATCH
#include "push.h"
#endif
#if REORGANIZETAGS_PATCH
#include "reorganizetags.h"
#endif
#if RESTARTSIG_PATCH
#include "restartsig.h"
#endif
#if RIODRAW_PATCH
#include "riodraw.h"
#endif
#if ROTATESTACK_PATCH
#include "rotatestack.h"
#endif
#if ROUNDED_CORNERS_PATCH
#include "roundedcorners.h"
#endif
#if RENAMED_SCRATCHPADS_PATCH
#include "renamed_scratchpads.h"
#elif SCRATCHPADS_PATCH
#include "scratchpad.h"
#endif
#if SCRATCHPAD_ALT_1_PATCH
#include "scratchpad_alt_1.h"
#endif
#if SEAMLESS_RESTART_PATCH
#include "seamless_restart.h"
#endif
#if SELFRESTART_PATCH
#include "selfrestart.h"
#endif
#if SETBORDERPX_PATCH
#include "setborderpx.h"
#endif
#if SHIFTBOTH_PATCH || SHIFTSWAPTAGS_PATCH || SHIFTTAG_PATCH || SHIFTTAGCLIENTS_PATCH || SHIFTVIEW_PATCH || SHIFTVIEW_CLIENTS_PATCH
#include "shift.h"
#endif
#if SHIFTBOTH_PATCH
#include "shiftboth.h"
#endif
#if SHIFTSWAPTAGS_PATCH && SWAPTAGS_PATCH
#include "shiftswaptags.h"
#endif
#if SHIFTTAG_PATCH
#include "shifttag.h"
#endif
#if SHIFTTAGCLIENTS_PATCH
#include "shifttagclients.h"
#endif
#if SHIFTVIEW_PATCH
#include "shiftview.h"
#endif
#if SHIFTVIEW_CLIENTS_PATCH
#include "shiftviewclients.h"
#endif
#if SIZEHINTS_RULED_PATCH
#include "sizehints_ruled.h"
#endif
#if SORTSCREENS_PATCH
#ifdef XINERAMA
#include "sortscreens.h"
#endif // XINERAMA
#endif
#if STACKER_PATCH
#include "stacker.h"
#endif
#if STICKY_PATCH
#include "sticky.h"
#endif
#if SWALLOW_PATCH
#include "swallow.h"
#endif
#if SWAPFOCUS_PATCH && PERTAG_PATCH
#include "swapfocus.h"
#endif
#if SWAPTAGS_PATCH
#include "swaptags.h"
#endif
#if SWITCHCOL_PATCH
#include "switchcol.h"
#endif
#if TAB_PATCH
#include "tab.h"
#endif
#if TAGALL_PATCH
#include "tagall.h"
#endif
#if TAGALLMON_PATCH
#include "tagallmon.h"
#endif
#if TAGOTHERMONITOR_PATCH
#include "tagothermonitor.h"
#endif
#if TAGSWAPMON_PATCH
#include "tagswapmon.h"
#endif
#if TAPRESIZE_PATCH
#include "tapresize.h"
#endif
#if TOGGLEFULLSCREEN_PATCH
#include "togglefullscreen.h"
#endif
#if TOGGLETOPBAR_PATCH
#include "toggletopbar.h"
#endif
#if TRANSFER_PATCH
#include "transfer.h"
#endif
#if TRANSFER_ALL_PATCH
#include "transferall.h"
#endif
#if UNFLOATVISIBLE_PATCH
#include "unfloatvisible.h"
#endif
#if VANITYGAPS_PATCH
#include "vanitygaps.h"
#endif
#if WARP_PATCH
#include "warp.h"
#endif
#if WINVIEW_PATCH
#include "winview.h"
#endif
#if ZOOMSWAP_PATCH
#include "zoomswap.h"
#endif
#if XKB_PATCH
#include "xkb.h"
#endif
#if XRDB_PATCH && !BAR_VTCOLORS_PATCH
#include "xrdb.h"
#endif
/* Layouts */
#if BSTACK_LAYOUT
#include "layout_bstack.h"
#endif
#if BSTACKHORIZ_LAYOUT
#include "layout_bstackhoriz.h"
#endif
#if CENTEREDMASTER_LAYOUT
#include "layout_centeredmaster.h"
#endif
#if CENTEREDFLOATINGMASTER_LAYOUT
#include "layout_centeredfloatingmaster.h"
#endif
#if COLUMNS_LAYOUT
#include "layout_columns.h"
#endif
#if DECK_LAYOUT
#include "layout_deck.h"
#endif
#if FIBONACCI_DWINDLE_LAYOUT || FIBONACCI_SPIRAL_LAYOUT
#include "layout_fibonacci.h"
#endif
#if FLEXTILE_DELUXE_LAYOUT
#include "layout_flextile-deluxe.h"
#endif
#if GAPPLESSGRID_LAYOUT
#include "layout_gapplessgrid.h"
#endif
#if GRIDMODE_LAYOUT
#include "layout_grid.h"
#endif
#if HORIZGRID_LAYOUT
#include "layout_horizgrid.h"
#endif
#if MONOCLE_LAYOUT
#include "layout_monocle.h"
#endif
#if NROWGRID_LAYOUT
#include "layout_nrowgrid.h"
#endif
#if TILE_LAYOUT
#include "layout_tile.h"
#endif

