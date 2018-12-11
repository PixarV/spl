%ifndef ENTRY
%define ENTRY
    %define next 0
    %define n 3
    %macro colon n
	%%next: dq next
        db %1, 0
	%2: 
	%define next %%next
    %endmacro
%endif
