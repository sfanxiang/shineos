;boot/includes/std.inc
;shineos

%ifndef _boot_includes_std_inc_included_
	%define _boot_includes_std_inc_included_
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

;@comment, @endcomment support
	%define @comment %if 0
	%define @endcomment %endif
%endif
