# Tiny Search Engine (TSE)
### Overview
This was the culminating project for my ENGS 50: Software Development class taken during Fall '19 at Dartmouth College. TSE is a simple search engine written in C. TSE is based on three subsystems: 
1. Crawler  
Takes a source URL, depth, and a local directory as parameters and crawls the web starting at the source URL upto the specified depth. The crawled webpages are fetched and saved into numbered files in the specified directory. 
2. Indexer  
Takes a directory containing crawled webpages and a file name as parameters and creates an index of words present in the crawled webpages. The index is saved locally for later use by the querier.
3. Querier  
Answers user search queries and ranks them based off the index created by the indexer.
4. Concurrent Crawler  
Same functionality as the crawler but with much improved speed because of added support for multi-threading using POSIX threads. Webpages are concurrently retrieved using two shared resources: queue of webpages to be crawled and hash-table of seen URLs.


### Folder structure
#### tse/utils
* queue.c  
Source code for queue data strcture.
* hash.c  
Source code for hash table data structure.
* pageio.c  
Contains functions for saving/loading webpages to/from a local directory. Used by the crawler and the indexer.
* indexio.c  
Contains functions for saving/loading an index to/from a file. Used by the indexer.
* lqueue.c  
Source code for a queue implemented with mutex locks.
* lhash.c  
Source code for a hash table implemented with mutex locks.

#### tse/crawler
* crawler.c
* conc_crawler.c  
Multi-threaded implementation of the crawler using POSIX threads. Webpages are retrieved concurrently. User may define number of threads to use.

#### tse/indexer
* indexer.c

#### tse/querier
* query.c

#### tse/pages
Contains crawled webpages saved as files with numeric filenames.

### Subsystem descriptions and usages
#### crawler.c
Fetches a webpage from a given seed_url, extracts any embedded URLs, 
retrieves each of those pages iteratively, storing associated files in a 
designated directory as a numbered file. Crawling mechanism is a breadth-first search in the graph of the internet, with the seed_url as the source node. A hashtable of seen_urls is maintained to avoid crawling over and fetching the same webpage multiple times. Search is limited to a given depth. 

Usage:  
&emsp; &emsp; &emsp; `crawler <seedurl> <pagedir> <maxdepth>`  
* seedurl is the starting url where the crawl begins.
* pagedir is where the saved pages are stored.
* maxdepth is the maximum depth that the crawler may crawl; it should be nonnegative.

#### conc_crawler.c
Same functionality as crawler.c but is a multi-threaded implementation 
using pthreads. User can set number of threads to use.

Usage:  
&emsp; &emsp; &emsp; `conc_crawler <seedurl> <pagedir> <maxdepth> <numthreads>`

#### indexer.c 
Creates an index from all the crawled webpages that are stored in 
a local directory. The index is a hashtable of words. Each word has
associated with it a queue of documents in which the word is found.
Each element of this queue, along with the document ID, also stores
the number of occurrences of the word in that document.

The index file shall contain one line for each word in the index. 
Each line has the format:  
`<word> <docID1> <count1> <docID2> <count2> ….<docIDN> <countN>`  
where \<word> is a string of lowercase letters, \<docIDi> is a positive 
integer designating a document, \<counti> is a positive integer designating
the number of occurrences of \<word> in \<docIDi>; each entry should be 
placed on the line separated by a space.

Usage:  
&emsp; &emsp; &emsp; `indexer <pagedir> <indexnm>`

* pagedir is directory where the saved pages exist 
* indexnm is the output file where index will be saved.

#### query.c
The querier reads a query from the user, consults an index built
by the indexer, ranks the documents fetched by the crawler according
to their relevance, and prints a list of documents in rank order.

There is an option to run the querier in quiet mode [-q]. This allows
the user to input a text file of queries and specify an output file where
the query results will be stored by the querier.

Usage (non-quiet):  
&emsp; &emsp; &emsp; `query <pageDirectory> <indexFile>` 

Usage (quiet mode):  
&emsp; &emsp; &emsp; `query <pageDirectory> <indexFile> [-q] input_file output_file`      

* pageDirectory is the path to the directory containing the crawled webpages.
* indexFile is the path to the text file containing the saved index.
* [-q] sets the quiet mode on.
* input_file is the path to the text file containing user's queries.
* output_file is the path to the output text file where query results will be stored.

The querier checks if the specified indexFile exists. If it does not, the
querier calls on the indexer to build the index based off the crawled pages 
in the specified pageDirectory.

### Running it for yourself
Follow the following steps if you wish to run TSE:
1. Download the repository*, maintaining the same folder structure.
2. Run the crawler according to the usage specified above.
3. Run the indexer according to the usage specified above (this step is optional -- if an index is not created by the time step 4. is executed, the querier will automatically call on the indexer to build the index).
4. Run the querier according to the usage specified above, either in interactive mode (default) or quiet mode. 

*Please contact musabshakeel@gmail.com if you would like access to the code* 




*Collaborators: Selim Hassairi*  
*Course taught by Professor Stephen Taylor*
