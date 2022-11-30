%define NEXT_ELEMENT 0

%macro colon 2
    %ifid %2
        %2: dq NEXT_ELEMENT
        %define NEXT_ELEMENT %2
    %else
        %error "Second argument should be an id"
    %endif
    %ifstr %1
        db %1, 0
    %else
        %error "First argument should be a string"
    %endif
%endmacro