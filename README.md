# Tiny Search Engine

## utils
* queue.c X
* hash.c X
* indexio.c
* pageio.c X
* lqueue.c X
* lhash.c X

## crawler
* crawler.c
* conc_crawler.c

## indexer
* indexer.c

## querier
* query.c

### crawler.c
Description:
    Fetches a webpage from a given seed_url, extracts any embedded URLs, 
    retrieves each of those pages iteratively, storing associated files in a 
    designated directory as a numbered file. Search is limited to a given depth

    Usage: 
        crawler <seedurl> <pagedir> <maxdepth> 

    where: 
    • seedurl is the starting url where the crawl begins
    • pagedir is where the saved pages are stored
    • maxdepth is the maximum depth that the crawler may crawl; it should be nonnegative

### conc_crawler.c
Description:
    Same functionality as crawler.c but is a multi-threaded implementation 
    using pthreads. User can set number of threads to use
    
    The crawler has the following usage: 
        usage: crawler <seedurl> <pagedir> <maxdepth> <numthreads>

### indexer.c
Description: 
    Creates an index from all the crawled webpages that are stored in 
    a local directory. The index is a hashtable of words. Each word has
    associated with it a queue of documents in which the word is found.
    Each element of this queue, along with the document ID, also stores
    the number of occurrences of the word in that document.

    The index file shall contain one line for each word in the index. 
    Each line has the format: 
        <word> <docID1> <count1> <docID2> <count2> ….<docIDN> <countN> 
    where <word> is a string of lowercase letters, <docIDi> is a positive 
    integer designating a document, <counti> is a positive integer designating
    the number of occurrences of <word> in <docIDi>; each entry should be 
    placed on the line separated by a space
    
    Usage: 
            indexer <pagedir> <indexnm> 
    
    where pagedir is directory where the saved pages exist and indexnm 
    is the output file where index will be saved.

### query.c
Description:
    The querier reads a query from the user, consults an index built
    by the indexer, ranks the documents fetched by the crawler according
    to their relevance, and prints a list of documents in rank order.

    There is an option to run the querier in quiet mode ([-q]). This allows
    the user to input a text file of queries and specify an output file where
    the query results will be stored by the querier.

    Usage (non-quiet):
        query <pageDirectory> <indexFile> 

    Usage (quiet mode): 
        query <pageDirectory> <indexFile> [-q] input_file output_file      
    
    where: 
    • pageDirectory is the path to the directory containing the crawled webpages
    • indexFile is the path to the text file containing the saved index
    • [-q] sets the quiet mode on
    • input_file is the path to the text file containing user's queries
    • output_file is the path to the output text file where query results will be 
      stored

    The querier checks if the specified indexFile exists. If it does not, the
    querier calls on the indexer to build the index based off the crawled pages 
    in the specified pageDirectory.