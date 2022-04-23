;Tabulatorgr��e:	3
;Kommentare:																;ab Spalte 60

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	Pure C Startup Code
;
;	ARGV und die �bergabe langer Dateinamen in Hochkommata wird unterst�tzt.
;  (c) und alle Rechte by Behne und Behne Software, Wunstorf.
;
;	07.10.95
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;zu exportierende Referenzen

						EXPORT	exit										;void	exit( WORD status );
						EXPORT	__exit
			
						EXPORT	_BasPag									;Adresse der Baspage
						EXPORT	_StkLim									;Untergrenze des Stacks
						EXPORT	_PgmSize									;Gr��e des Programms (TPA)
						EXPORT	_app										;Flag f�r Accessory (0) oder Applikation (1)
						EXPORT	errno										;globale Fehlervariable
						EXPORT	_AtExitVec
						EXPORT	_FilSysVec
						EXPORT	_RedirTab
			
						EXPORT	__text
						EXPORT	__data
						EXPORT	__bss

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;zu importierende Referenzen

						IMPORT	main										;WORD main( WORD argc, BYTE **argv, BYTE **envp );
						IMPORT	_fpumode									;Art der FPU
						IMPORT	_StkSize									;Gr��e des Stacks
						IMPORT	_FreeAll									;Funktion zur Speicherfreigabe

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;die BASEPAGE-Struktur

						OFFSET	0

p_lowtpa:			ds.l	1												;Adresse der TPA
p_hitpa:				ds.l	1												;erstes Byte nach der TPA

p_tbase:				ds.l	1												;Adresse des TEXT-Segements
p_tlen:				ds.l	1												;L�nge des TEXT-Segments

p_dbase:				ds.l	1												;Adresse des DATA-Segments
p_dlen:				ds.l	1												;L�nge des DATA-Segments

p_bbase:				ds.l	1												;Adresse des BSS-Segments
p_blen:				ds.l	1												;L�nge des BSS-Segments

p_dta:				ds.l	1												;Zeiger auf die Default-DTA

p_parent:			ds.l	1												;Zeiger auf die Basepage des aufrufenden Prozesses

p_resrvd0:			ds.l	1												;reserviert

p_env:				ds.l	1												;Zeiger auf die Environment-Strings
p_resrvd1:			ds.b	80												;reserviert
p_cmdlin:			ds.b	128											;Kommandozeile

sizeof_BASEPAGE:	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

						TEXT
__text:																		;Startadresse des TEXT-Segments

						bra.b		start_me_up								;den Header �berspringen

						dc.l		_RedirTab								;Zeiger auf die Umlenkungstabelle
						dc.l		_StkSize									;Gr��e des Stacks

						ALIGN	16

						dc.b	'Pure C',0									;Copyright-Hinweis

						ALIGN	16

start_me_up:		move.l	a0,d0										;Accessory?
						bne.b		is_acc

						move.l	4(sp),a3									;Zeiger auf die Basepage
						moveq		#1,d0										;Applikation
						bra.b		is_app

is_acc:				movea.l	a0,a3										;Zeiger auf die Basepage
						clr.w		d0											;Accessory

is_app:				move.l	a3,_BasPag								;Zeiger auf die Basepage
						move.w	d0,_app									;Flag Accessory/Applikation

;Speicherbedarf ermitteln

						move.l	p_tlen(a3),d1							;L�nge des TEXT-Segments
						add.l		p_dlen(a3),d1							;L�nge des DATA-Segments
						add.l		p_blen(a3),d1							;L�nge des BSS-Segments
						add.l		#sizeof_BASEPAGE,d1					;L�nge der Basepage
						move.l	d1,_PgmSize								;Programml�nge

;Stack setzen
						move.l	d1,d2										;L�nge
						add.l		a3,d2										;Zeiger auf die Basepage
						and.b		#$fc,d2									;auf Langwortgrenze abrunden
						move.l	d2,sp										;Adresse des Stacks

						tst.w		d0											;Accessory?
						beq		chk_fpu

;Speicher freigeben

						move.l	d1,-(sp)									;Speicherbedarf
						move.l	a3,-(sp)									;Adresse der Basepage
						clr.w		-(sp)										;reserviert
						move.w	#74,-(sp)								;Mshrink
						trap		#1
						lea.l		12(sp),sp

;Environment-Strings untersuchen

						move.l	sp,d0
						sub.l		#_StkSize-4,d0
						and.b		#$fc,d0									;auf Langwortgrenze ausrichten
						movea.l	d0,a1										;envp[] beginnt bei der niedrigsten Stackadresse
						move.l	a1,a4										;merken
						move.l	p_env(a3),a2							;Zeiger auf die Environment-Strings

scan_env_loop:		move.l	a2,(a1)+									;Adresse in envp[] sichern
						movea.l	a2,a5										;Adresse sichern
						tst.b		(a2)+										;Ende der Environment-Strings?
						beq.b		scan_env_end
						
scan_env_str:		tst.b		(a2)+										;bis zum String-Ende
						bne.b		scan_env_str

						movep.w	0(a5),d0
						swap		d0
						movep.w	1(a5),d0									;'ARGV' ergibt in diesem Fall 'AGRV'
						cmpi.l	#'AGRV',d0								;'ARGV' ergibt in diesem Fall 'AGRV'
						bne.b		scan_env_loop
						cmpi.b	#'=',4(a5)								;'=' ?
						bne.b		scan_env_loop
						cmpi.b	#127,p_cmdlin(a3)						;Kommandozeile 127 Bytes lang?
						bne.b		scan_env_loop

						clr.b		(a5)										;Environment terminieren (ARGV entfernen)
						clr.l		-4(a1)									;letzten envp[] l�schen 
						movea.l	a1,a5										;argv[] beginnt direkt hinter envp[]
						moveq		#0,d3										;argc ist 0
						move.l	a2,(a1)+									;Zeiger auf den ersten String

scan_ARGV:			tst.b		(a2)+										;Ende des Strings suchen
						bne.b		scan_ARGV
						
						move.l	a2,(a1)+									;Zeiger auf das n�chste Argument
						addq.w	#1,d3										;argc hochz�hlen
						tst.b		(a2)										;letzter Environment-String?
						bne.b		scan_ARGV

						bra		scan_end

scan_env_end:		clr.l		-4(a1)									;letzten envp[] l�schen

;Kommandozeile untersuchen

scan_cmd_line:		lea		p_cmdlin(a3),a0						;Zeiger auf die Kommandozeile
						move.b	(a0),d1									;L�nge der Kommandozeile
						ext.w		d1
						movea.l	a1,a5										;argv[] merken

						move.l	a0,(a1)+									;argv[0]: Zeiger auf der ersten String
						clr.b		(a0)+										;Leerstring
						moveq		#1,d3										;argc
						move.l	a0,(a1)+									;argv[1]: Zeiger auf den zweiten String

						moveq		#' ',d4									;Leerzeichen
						moveq		#39,d5									;Hochkomma
						moveq		#'"',d6									;Anf�hrungszeichen
						
						bra.b		scan_next
						
scan_cmd_loop:		move.b	(a0)+,d0
						cmp.b		d4,d0										;Leer- oder Trennzeichen?
						bhi.b		scan_squote
	
						tst.b		-2(a0)									;ist das vorhergehende Byte ein Nullbyte?
						bne.b		scan_eos									;Ende eines Strings

						addq.l	#1,-4(a1)								;Adresse des Strings korrigieren
						bra.b		scan_term								;dann das Leerzeichen ignorieren
						
scan_squote:		cmp.b		d5,d0										;Dateiname in ' Hochkommata?
						bne.b		scan_dquote

						move.l	-4(a1),d0
						addq.l	#1,d0
						cmp.l		d0,a0										;Hochkomma mitten im Text?
						bne.b		scan_dquote

						addq.l	#1,-4(a1)								;Adresse des Strings korrigieren

scan_snglquote:	cmp.b		(a0)+,d5									;' Hochkomma?
						dbeq		d1,scan_snglquote
						subq.w	#1,d1										;Ende des Strings?
						bmi.b		scan_end
						beq.b		scan_eos									;es folgen keine weiteren Zeichen

						cmp.b		(a0),d5									;zus�tzliches Hochkomma?
						bne.b		scan_eos									;sonst Ende des Dateinamens?
						
						movea.l	-(a1),a2									;Zeiger auf den Anfang des Strings						
						movea.l	a0,a3										;Zeiger auf das zweite Hochkomma
						
scan_move_args:	move.b	-2(a3),-(a3)							;String um ein Byte verschieben
						cmpa.l	a2,a3
						bhi.b		scan_move_args

						addq.l	#1,a0										;Zeiger hinter das zweite Hochkomma
						subq.w	#1,d1										;L�nge der Kommandozeile verringern
						addq.l	#1,(a1)+									;Adresse des Strings korrigieren
						bra.b		scan_snglquote							;nach dem n�chsten Hochkomma suchen

scan_dquote:		cmp.b		d6,d0										;Dateiname in Anf�hrungszeichen?
						bne.b		scan_next

						addq.l	#1,-4(a1)								;Adresse des Strings korrigieren

scan_dblquote:		cmp.b		(a0)+,d6									;Anf�hrungszeichen?
						dbeq		d1,scan_dblquote
						subq.w	#1,d1										;Ende des Strings?
						bmi.b		scan_end

scan_eos:			move.l	a0,(a1)+									;Startadresse des neuen Strings				
						addq.w	#1,d3										;argc erh�hen
scan_term:			clr.b		-1(a0)									;String terminieren
						
scan_next:			subq.w	#1,d1
						bpl.b		scan_cmd_loop

						tst.b		-1(a0)									;ist letzte Byte ein Nullbyte?
						beq.b		scan_end									;dann das Leerzeichen ignorieren

						addq.w	#1,d3										;argc erh�hen
						clr.b		(a0)										;String terminieren
						addq.l	#4,a1

scan_end:			movea.l	a1,a6										;untere Stackgrenze, stack limit
						clr.l		-(a1)										;letzten argv[] l�schen

chk_fpu:				clr.w		_fpumode									;keine Atari FPU

						lea		256(a6),a6								;256 Bytes f�r Aufrufe der Libraries addieren
						move.l	a6,_StkLim								;niedrigste Stackadresse

						clr.w		errno										;globale Fehlervariable (extlib) l�schen
						clr.l		_AtExitVec								;Vektor f�r atexit() (stdlib-Funktion) l�schen
						clr.l		_FilSysVec								;Vektor f�rs Terminieren des Dateisystems l�schen

;main() anspringen

						move.w	d3,d0										;argc
						movea.l	a5,a0										;argv
						movea.l	a4,a1										;envp
						
						jsr		main										;WORD	main( WORD argc, BYTE **argv, BYTE **envp );


;void	exit( WORD status );

exit:					move.w	d0,-(sp)									;R�ckgabewert f�r Pterm() sichern

						move.l	_AtExitVec,d0							;sind mit atexit() Funktionen eingetragen worden?
						beq.b		__exit
			
						movea.l	d0,a0
						jsr		(a0)

__exit:				move.l	_FilSysVec,d0							;m�ssen noch Dateien geschlossen oder zur�ckgeschrieben werden?
						beq.b		exit_free
			
						movea.l	d0,a0
						jsr		(a0)										;Datei-Funktionen aufrufen

exit_free:			jsr		_FreeAll									;Speicherbl�cke freigeben

						move.w	#76,-(sp)								;Pterm()
						trap		#1


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
						DATA
__data:																		;Startadresse des DATA-Segments

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
						BSS
__bss:																		;Startadresse des BSS-Segments

_BasPag:				ds.l	1												;Zeiger auf die Basepage
_app:					ds.w	1												;Flag 0: Accessory / 1: Applikation
_StkLim:				ds.l	1												;untere Stackgrenze
_PgmSize:			ds.l	1												;L�nge des Programms (der TPA)
_RedirTab:			ds.l	6												;Tabelle f�r Ausgabe-Umlenkung

errno:				ds.w	1												;globale Fehlervariable (extlib)
_AtExitVec:			ds.l	1												;Vektor f�r atexit() (stdlib-Funktion)
_FilSysVec:			ds.l	1												;Vektor f�rs Terminieren des Dateisystems

						END
