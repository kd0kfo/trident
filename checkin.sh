#!/bin/sh

cat  > src/revision_string.c <<EOF
const char revision_string[] = "\$Id\$";

#if 0 //Revision Log
EOF

svn log >> src/revision_string.c

cat  >> src/revision_string.c <<EOF
#endif //Revision Log
EOF

svn ci $@