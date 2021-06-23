- [x] A) Raise the notes-and-reminders size
- [x] B) `config.def.h`: ver a lista de todos os active patches em `README.md` marcados com [C], e colocar prefixos para que apareçam nas cheatsheets nos que possuírem bindings:
	[x] CYCLELAYOUTS_PATCH
	[x] DECORATION_HINTS_PATCH
	[x] FULLSCREEN_PATCH
	[x] MAXIMIZE_PATCH
	[x] MOVERESIZE_PATCH
	[x] MOVESTACK_PATCH
	[x] ONLYQUITONEMPTY_PATCH
	[x] PUSH_PATCH
	[x] SCRATCHPADS_PATCH
	[x] SETBORDERPX_PATCH
	[x] SHIFTVIEW_PATCH
	[x] STACKER_PATCH
	[x] STICKY_PATCH
	[x] SWALLOW_PATCH
	[x] VANITYGAPS_PATCH
	[x] XRDB_PATCH (`~/.Xresources`)
- [x] C) `config.def.h`: detectar os atalhos duplicados e remover as duplicidades com novos atalhos (usar os scripts em `/storage/src/devops/bin/dwm-print-cheatsheet.sh` e `/storage/src/devops/bin/dwm-print-all-mapped-keys.sh` para detectá-los).
- [x] D) Remover atalhos que não fazem mais sentido da cheatsheet
- [x] E) Recompilar o dwm e testar os bindings de "cyclelayout" e "shiftview" (ver a cheatsheet)
- [-] F) Notifications:
  	- [-] binding no dmenu para rodar o `toggle-stalonetray.sh`
  	- [ ] criar um menu dmenu para lidar com as notificações do dunst (ver comandos disponíveis no dunstctl). Colocar no dwmblocks se as notificações estão ON/OFF.
- [ ] G) Configurar cyberpunk colors (maybe check the darkula theme for inspiration?) - to do that, edit `~/.Xresources` and use the color names prefixed with `dmm.*` defined in `/storage/src/dwm-flexipatch/patch/xrdb.c`.
- [ ] H) Mergear essa branch na master.

