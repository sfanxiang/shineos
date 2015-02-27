%ifndef STD_INC
%define STD_INC

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

@@

%endif
