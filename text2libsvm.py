"""
convert train.tsv to libsvm, save vocabulary, skip empty lines:
text2libsvm.py train.tsv train.txt -s vocab.txt

convert test.tsv to libsvm using previously saved vocabulary:
text2libsvm.py test.tsv test.txt -l vocab.txt -t 1
"""

import sys
import csv
import json
import argparse
import numpy as np
from sklearn.datasets import dump_svmlight_file
from sklearn.feature_extraction.text import CountVectorizer

###

def load_vocabulary( input_file ):
        i_f = open( input_file )
        feature_names = i_f.read()
        feature_names = feature_names.split( "\n" )
        return feature_names

def save_vocabulary( output_file, feature_names ):
        o_f = open( output_file, 'wb' )
        feature_names = "\n".join( feature_names )
        o_f.write( feature_names )


# a generator wrapper to get data from a file
def iterable( reader, test_set, column_name ):

        global labels
        global url_id
        n = 0
        for line in reader:
                n += 1
                if n % 1000 == 0:
                        print n

                line.pop( 0 )   # url
                url_id.append(line.pop( 0 ))   # urlId

                boilerplate = line.pop( 0 )
                boilerplate = json.loads( boilerplate )

                """
                try:
                        a_title = boilerplate['title']
                except e:
                        continue
                """
                
                try:
                        column_b = boilerplate["body"] if boilerplate.has_key("body") else ""
                        column_u = boilerplate["url"] if boilerplate.has_key("url") else ""
                        column_t = boilerplate["title"] if boilerplate.has_key("title") else ""
                        
                        column = "%s %s %s" % (column_b, column_u, column_t)
                except :
                        column = ' '
                      
                if column is None:
                        column = ' '      # so that the vectorizer won't ignore it

                #if column.strip() == '' and not test_set:
                        #continue

                if not test_set:                
                        label = line[-1]
                        labels.append( int( label ))

                column = column.lower().encode( 'ascii', 'ignore' ) 
                #print body
                yield column

###

print __doc__

parser = argparse.ArgumentParser()
parser.add_argument( "input_file" )
parser.add_argument( "output_file" )
parser.add_argument( "-c", "--column", help = "column of textual content to be processed" )
parser.add_argument( "-s", "--save-vocabulary-file", help = "optional file to save vectorizer vocabulary", default = None )
parser.add_argument( "-l", "--load-vocabulary-file", help = "optional file to load vectorizer vocabulary from", default = None )
parser.add_argument( "-t", "--test-set", help = "if an input file is a test set", default = None )

args = parser.parse_args()

#stop_words = "a,able,about,across,after,all,almost,also,am,among,an,and,any,are,as,at,be,because,been,but,by,can,cannot,could,dear,did,do,does,either,else,ever,every,for,from,get,got,had,has,have,he,her,hers,him,his,how,however,i,if,in,into,is,it,its,just,least,let,like,likely,may,me,might,most,must,my,neither,no,nor,not,of,off,often,on,only,or,other,our,own,rather,said,say,says,she,should,since,so,some,than,that,the,their,them,then,there,these,they,this,tis,to,too,twas,us,wants,was,we,were,what,when,where,which,while,who,whom,why,will,with,would,yet,you,your"

#stop_words = stop_words.split(",")

i_f = open( args.input_file )
reader = csv.reader( i_f, delimiter = "\t" )

reader.next()
labels = []
url_id = []

#i = 0
#X =  iterable(reader, args.test_set, args.column)

#train_or_test = "train" if not args.test_set else "test"
#file_name = "%s_%s.lac" % (args.column, train_or_test)

#f = open(file_name, "w")
#for x in X:
  #x.replace(" ", " w=")
  
  #label = labels[i] if not args.test_set else 1
  #line = x.replace(",","")
  #line = line.lower().split(" ")
  
  #line = [x for x in line if x not in stop_words]
    
  #line = " w=".join(line)
  #line = "%s CLASS=%s w=%s\n" % (url_id[i], label, line)
  #f.write(line)
  #i += 1
  
#f.close()

if args.load_vocabulary_file:
        
        vocabulary_file = "%s_%s" % (args.column, args.load_vocabulary_file)
        print "loading vocabulary from", vocabulary_file
        
        vocabulary = load_vocabulary( vocabulary_file )
        vectorizer = CountVectorizer( min_df = 3, max_df = 0.9, strip_accents = 'unicode', binary = True, vocabulary = vocabulary )
        X = vectorizer.transform( iterable( reader, args.test_set, args.column ))

else:
        vectorizer = CountVectorizer( min_df = 3, max_df = 0.9, strip_accents = 'unicode', binary = True )
        X = vectorizer.fit_transform( iterable( reader, args.test_set, args.column ))

        if args.save_vocabulary_file:
                vocabulary_file = "%s_%s" % (args.column, args.save_vocabulary_file)
                print "saving vocabulary to", vocabulary_file
                
                feature_names = vectorizer.get_feature_names()
                save_vocabulary( vocabulary_file, feature_names )
                



print X.shape
# print vectorizer.get_feature_names()

if not args.test_set:
        y = np.asarray( labels )
else:
        y = np.ones( X.shape[0], dtype = int )

output_file = "%s_%s" % (args.column, args.output_file)
dump_svmlight_file( X, y, f = output_file, zero_based = False )