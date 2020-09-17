#!/bin/sh

source ./bst.$1.env || exit 1

d2 -c BSTCfg[0].URL ${BST_S3_URL}


