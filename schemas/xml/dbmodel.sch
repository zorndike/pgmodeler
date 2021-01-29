# XML definition for database model
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
<?xml $sp version="1.0" $sp encoding="UTF-8"?> $br
[<!--] $br
[CAUTION: Do not modify this file unless you know what you are doing.] $br
[         Unexpected results may occur if the code is changed deliberately.] $br
[-->] $br
<dbmodel [ pgmodeler-ver=] "{pgmodeler-ver}" [ use-changelog]= "{use-changelog}"
           
 %if {author} %then [ author=] "{author}" %end

 %if {protected} %then 
  [ protected=] "true"
 %end

 %if {last-position} %then
  [ last-position=] "{last-position}"
 %end

 %if {last-zoom} %then
  [ last-zoom=] "{last-zoom}"
 %end
 
 %if {max-obj-count} %then
  [ max-obj-count=] "{max-obj-count}"
 %end
 
 %if {default-schema} %or {default-owner}  %then
    $br $tb

    %if {default-schema} %then
      [ default-schema=] "{default-schema}"
    %end

    %if {default-owner} %then
      [ default-owner=] "{default-owner}"
    %end
 %end

 %if {default-collation} %or {default-tablespace} %then
    $br $tb

    %if {default-collation} %then
      [ default-collation=] "{default-collation}"
    %end

    %if {default-tablespace} %then
      [ default-tablespace=] "{default-tablespace}"
    %end

 %end
 
 %if {layers} %then
    $br $tb
    [ layers]="{layers}"
    
    [ active-layers]=
    %if {active-layers} %then 
        "{active-layers}" 
    %else
        "" 
    %end 
 %end

 > $br
 %if {objects} %then {objects} %end
 %if {permission} %then {permission} %end
 %if {changelog} %then {changelog} %end
 </dbmodel> $br
