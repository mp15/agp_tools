library(Gviz)

data <- read.table('cns_p_ctg.purged.fa.cut_BSPQI_BSSSI_0kb_0labels_NGS_contigs_HYBRID_Export.agp',
  col.names = c('Obj_Name','Obj_Start','Obj_End','PartNum','Compnt_Type','CompntId_GapLength','CompntStart_GapType','CompntEnd_Linkage','Orientation_LinkageEvidence'));
track1_data <- data[data$Obj_Name == 'Super-Scaffold_1' & data$Compnt_Type == 'W',]

atrack <- AnnotationTrack(start=track1_data$Obj_Start, end=track1_data$Obj_End, id = track1_data$CompntId_GapLength, chromosome = '1', name = 'Super-Scaffold_1',)

gtrack <- GenomeAxisTrack()

plotTracks(atrack)
plotTracks(list(gtrack, atrack))

data_b <- read.table('draft4h_unzip_p_ctg.purgedups.hic.fasta.fixed.agp',
col.names = c('Obj_Name','Obj_Start','Obj_End','PartNum','Compnt_Type','CompntId_GapLength','CompntStart_GapType','CompntEnd_Linkage','Orientation_LinkageEvidence'));
#data_b[data_b$CompntId_GapLength ==  000332F|arrow',]
#data_b[data_b$Obj_Name == 'scaffHiC_583',]

track2_data <- data_b[data_b$Obj_Name == 'scaffHiC_583' & data_b$Compnt_Type == 'W',]
#Flip the track
NewStart = 127513548 - track2_data$Obj_End
NewEnd = 127513548 - track2_data$Obj_Start
track2_data$Obj_Start <- NewStart
track2_data$Obj_End <- NewEnd
NewOrient <- track2_data$Orientation_LinkageEvidence;
NewOrient[NewOrient == '+'] <- '-';
NewOrient[NewOrient == '-'] <- '+';
track2_data$Orientation_LinkageEvidence <- NewOrient
track2_data <- track2_data[order(track2_data$Obj_Start),]

btrack <- AnnotationTrack(start=track2_data$Obj_Start, end=track2_data$Obj_End, id = track2_data$CompntId_GapLength, chromosome = '1', name = 'scaffHiC_583',)

plotTracks(list(gtrack, atrack, btrack),showFeatureId=TRUE)


#track2_data[order(track2_data$New_Start),]
#track1_data

#Align the beginning of track 1 with 2
track1_data$Obj_Start <- track1_data$Obj_Start + (3431031 - 1)
track1_data$Obj_End <- track1_data$Obj_End + (3431031 - 1)

#track1 has more accurate gap estimation so adjust track 2 again
track2_data[track2_data$Obj_Start > (3431031),'Obj_Start'] <- track2_data[track2_data$Obj_Start > (3431031), 'Obj_Start'] + (258031)
track2_data[track2_data$Obj_Start > (3431031),'Obj_End'] <- track2_data[track2_data$Obj_Start > (3431031),'Obj_End'] + (258031)

atrack <- AnnotationTrack(start=track1_data$Obj_Start, end=track1_data$Obj_End, id = track1_data$CompntId_GapLength, chromosome = '1', name = 'Super-Scaffold_1',)
btrack <- AnnotationTrack(start=track2_data$Obj_Start, end=track2_data$Obj_End, id = track2_data$CompntId_GapLength, chromosome = '1', name = 'scaffHiC_583',)
plotTracks(list(gtrack, atrack, btrack),showFeatureId=TRUE)
