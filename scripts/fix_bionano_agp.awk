BEGIN{OFS="\t";
} {
if ($5 == "W" && $6 ~ /_subseq_[0-9]+:[0-9]+/) {
$0 = gensub(/_subseq_([0-9]+):([0-9]+)\t[0-9]+\t[0-9]+\t/,"\t\\1\t\\2\t","g");
}
print
}
