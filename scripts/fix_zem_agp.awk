BEGIN{OFS="\t";
print "##agp-version 2.0";
print "# Organism:";
print "# Platform:";
print "# Model:";
print "# Enzyme(s):";
print "# BioSample:";
print "# BioProject:";
printf "# Obj_Name\tObj_Start\tObj_End\tPartNum\tCompnt_Type\tCompntId_GapLength\tCompntStart_GapType\tCompntEnd_Linkage\tOrientation_LinkageEvidence\n";
} {if ($5=="N"){$7="scaffold"; $8="yes";} print}
