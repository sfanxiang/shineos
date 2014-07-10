;boot/std.inc
;shineos

%ifndef _boot_std_inc_included_
	%define _boot_std_inc_included_
;@b, @f support
	%idefine @b %$B
	%idefine @f %$F
	
	%macro @@ 0
	%ifctx _anon_labels_
		%$F:
		%pop
	%endif
	%push _anon_labels_
	%$B:
	%endmacro

%endif
