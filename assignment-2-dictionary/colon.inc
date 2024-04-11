%define NEXT_NODE 0

%macro  colon 2
%ifid %2
	%2: dq NEXT_NODE
%else
	%error "Wrong argument: id"
%endif
%ifstr %1
	db %1,0
%else
	%error "Wrong argument: key"
%endif
%define NEXT_NODE %2
%endmacro
