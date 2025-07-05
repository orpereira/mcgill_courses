from gensim.models import KeyedVectors
from gensim.models import Word2Vec
import numpy as np
from sklearn.utils.extmath import randomized_svd

import re
from collections import Counter

''' ---------- PART 1 ---------- '''

def top_k_unigrams(tweets, stop_words, k):
    '''A helper function that is used to determine the most frequently occurring words in the corpus. 
    These will later constitute the dimensions of the vector space model.

    Given a list of tweets, compute the frequency of each word. 
    Ignore stop words and words that do not start with [a-z#] (Use regex to implement this). 
    If k is positive, return the top k most frequently occurring words, along with their counts. 
    If k is -1, return all words and their frequencies (filtering based on stopwords and  [a-z#] is 
    still applicable).

    Parameters:
        tweets (list of str): Cleaned and tokenized tweets from Assignment 1
        stop_words (list of str): Words not to be considered while deciding most frequent words.
        k (int): Number of most frequent words to return, along with their counts. If k is -1, then 
                 return the entire count dictionary without any filtering.

    Returns:
        top_k_words (dict of {str: int}): A dictionary of top-k words with their frequency counts. 
                                          It will have all words with their counts if k=-1
    '''
    
    unigram_counter = Counter()
    
    for tweet in tweets:
        words = tweet.split()
        for word in words:
            # exclude words starting with non-letters # and stop words
            if (not re.match(r'^[a-z#]', word)) or (word in stop_words):
                continue

            # increase frequency of word by 1
            unigram_counter[word] += 1

    if k == -1:
        return unigram_counter
    
    elif k > 0:
        # sorting the Counter by the count values by using most common
        return Counter(dict(unigram_counter.most_common(k)))
    
    else:
        print("Invalid value of k")
        return None

def context_word_frequencies(tweets, stop_words, context_size, frequent_unigrams):
    ''' A helper function that is used to get the frequency of pairs of words that will eventually 
    be used for PMI calculations.
    Given a list of tweets, compute the frequency of each word with other words in the context. 
    We define context as up to context_size words to the left and right. 
    Ignore stop words and words that do not start with [a-z#] (Use regex to implement this). 
    Also, ignore words that are not in the top 1,000 frequent words (computed from the previous function).

    Return a dictionary of the form {(word_1, word_2): freq}, where word_2 is the context word.
    Note that the stopwords, [a-z#], and top-1000 restrictions do not apply to word_1.
    
    Parameters:
        tweets (list of str): Cleaned and tokenized tweets from Assignment 1
        stop_words (list of str): Words not to be considered even if they occur in the context window. 
        context_size (int): Number of words to be considered in context on left and right 
        frequent_unigrams (list of str): Top-1000 Frequent unigrams computed in the previous function 
                                         (converted to a list).

    Returns: 
        context_counter (dict of {(str, str): int}): A dictionary of frequencies of pairs of words
    '''

    context_counter = Counter()
    
    for tweet in tweets:
        words = tweet.split()
        for i in range(len(words)):

            # not imposing conditions on word1 in (word1, word2) pair
            word = words[i]

            # get the context window, including cases for if the window goes beyond tweet bounds
            for j in range(max(0, i - context_size), min(len(words), i + context_size + 1)):
                if j == i: continue
                
                context_word = words[j]

                # check that context word obeys conditions 
                if ((not re.match(r'^[a-z#]', context_word)) or (context_word in stop_words) or 
                    (context_word not in frequent_unigrams)):
                    continue
                
                # increase frequency of word pair by 1
                context_counter[(word, context_word)] += 1

    return context_counter

def pmi(word1, word2, unigram_counter, context_counter):
    '''A helper function to calculate Pointwise Mutual Information for a pair of words based on 
    unigram and bigram counts. If the unigram or bigram has not been observed, assume a pseudo count 
    of 1. However, the total count of unigrams will be fixed and will be independent of whether the 
    actual count is used or the pseudo count.
    
    Parameters:
        word_1 (str): 1st word of the pair for which PMI has to be calculated
        word_2 (str): 2nd word of the pair for which PMI has to be calculated
        unigram_counter (dict of {str: int}): All words in the corpus along with their frequencies
        context_counter (dict of {(str, str): int}): Frequencies of pairs of words as computed in 
                                                     the previous function.
    Returns:
    pmi (float): PMI value for word_1 and word_2
    '''

    # Using formula from lecture 4, slide 17
    # PMI(x,y) = log(freq(x,y)N / freq(x)freq(y))

    # get the frequency of the pair of words using get function, if not found = 1
    freq_xy = context_counter.get((word1, word2), 1)

    # we can get the total number of words by adding the counts of all unigrams
    N = sum(unigram_counter.values())

    # imposing pseudo-1 value if word is not found to avoid dividing by zero
    freqx = unigram_counter.get(word1, 1)
    freqy = unigram_counter.get(word2, 1)
    
    # careful! needs to be log2 here! got this from slack messages on class forum
    pmi = np.log2((freq_xy * N) / (freqx * freqy))
    
    return pmi

def build_word_vector(word1, frequent_unigrams, unigram_counter, context_counter):
    '''A helper function to construct a word vector based on PMI values using statistics computed 
    from previous functions. The top k frequent words define the dimensions of the vector space 
    model.
    
    Parameters: 
        word_1 (str): Word for which word vector has been computed
        frequent_unigrams (list of str): Top-k unigrams
        unigrams_counter (dict of {str: int}): All words in the corpus along with their frequencies
        context_counter (dict of {(str, str): int}): Frequencies of pairs of words
    
    Returns:
        word_vector (dict of {str: float}): Word vector of word_1 represented as a dictionary
    '''
    word_vector = {}

    # iterate over all frequent unigrams
    for word2 in frequent_unigrams:
        # check if pair appears in context_counter
        if (word1, word2) not in context_counter: pmi_val = float(0)

        else: 
            # calculate the PMI value for the word pair
            pmi_val = pmi(word1, word2, unigram_counter, context_counter)

        word_vector[word2] = pmi_val

    return word_vector

def get_top_k_dimensions(word1_vector, k):
    ''' This helper functions helps to visualize the dimensions (and by extension context words) 
    most relevant to a given word, using its vector. The higher the value of a particular dimension 
    in the word vector, the greater the relevance.
    
    Parameters:
        word1_vector (dict of {str: float}): Word vector
        k (int): Top-k dimensions to be returned

    Returns:
        top_k_dimensions (dict of {str: float}): Top-k dimensions of the input word vector based on 
                                                 dimension values.
    '''
    return dict(Counter(word1_vector).most_common(k))

def get_cosine_similarity(word1_vector, word2_vector):
    '''This is the final piece that will complete the vector space model for word representation. 
    Given word vectors of two words, their relatedness is measured using cosine similarity.

    Parameters:
        word1_vector (dict of {str: float}): Word vector of the 1st word
        word2_vector (dict of {str: float}): Word vector of the 2nd word

    Returns:
        cosine_sim_score (float): Cosine similarity between the two word vectors
    '''
    # using formula from lecture 4, slide 15
    # cos(x,y) = sum(xi*yi) / sqrt(sum(xi^2) * sum(yi^2)), with all sums from i to F
    
    # assumes inputs are already sorted
    # get the numerator and denominator for the cosine similarity
    word1_numerical = np.array(list(word1_vector.values()))
    word2_numerical = np.array(list(word2_vector.values()))

    # calculate the numerator and denominator
    numerator = np.dot(word1_numerical, word2_numerical)    
    # calculate the denominator
    denominator = np.linalg.norm(word1_numerical) * np.linalg.norm(word2_numerical)

    # calculate the cosine similarity
    cosine_sim_score = numerator / denominator
    return cosine_sim_score

''' ---------- PART 2 ---------- '''

def get_most_similar(word2vec, word, k):
    '''Given a pre-loaded word2vec model and a word, the function should return k most similar words 
    from the vocabulary based on the cosine similarity. If the word is not in the vocabulary, the 
    function should return an empty list.

    Parameters:
        word2vec (gensim.models.keyedvectors.KeyedVectors): Pre-loaded word2vec model from gensim
        word (str): Word for which similar words have to be found
        k (int): Number of similar words to return

    Returns:
        similar_words (list of (str, float)): List of similar words along with cosine similarity 
                                              score.
    '''

    # if the word is not in the vocabulary, return an empty list
    if word not in word2vec:
        return []
    
    # get the most similar words using the gensim function
    # reference: https://tedboy.github.io/nlps/generated/generated/gensim.models.Word2Vec.most_similar.html
    similar_words = word2vec.most_similar(word, topn=k)
    return similar_words

def word_analogy(word2vec, word1, word2, word3):
    ''' Word analog in this context means answering the question: X is to Y as A is to what?
    For example, Tokyo is to Japan as Paris is to what?. The answer is France.
    
    Parameters:
        word2vec (gensim.models.keyedvectors.KeyedVectors): Pre-loaded word2vec model from gensim
        word_1 (str): X, where X is to Y as A is to what?
        word_2 (str): Y, where X is to Y as A is to what
        word_3 (str): A, where X is to Y as A is to what?

    Returns:
        word_4 ((str, float)): Word that is the answer to the analogy task, along with its score as 
                               given by word2vec model.

    '''
    # using same reference as above, can do arithmetic on word vectors as seen in class
    # so here, word 1 : word 2 :: word 3 : word4 means word4 = word2 - word1 + word3
    # so we input them accordingly
    # resource: https://stackoverflow.com/questions/52364632/what-is-the-operation-behind-the-word-analogy-in-word2vec
    word_4 = word2vec.most_similar(positive = [word2, word3], negative = [word1])[0]
    
    return word_4

''' ---------- PART 3 ---------- '''
def cos_sim(A, B):
    '''This should be a function that returns the cosine similarity of two numerical vectors. 
    Importantly, unlike the get_cosine_similarity function from Question 1, this function should 
    take as its inputs two Numpy arrays or lists of numerical values, rather than two dictionaries 
    of {str: float}.
    
    Parameters: 
        A (np.ndarray or list of numerical values): The first vector to be compared.
        B (np.ndarray or list of numerical values): The other vector to be compared.

    Returns:
        cosine_similarity (float): The cosine similarity of vectors A and B.
    '''
    return np.dot(A, B) / (np.linalg.norm(A) * np.linalg.norm(B))

def get_cos_sim_different_models(word, model1, model2, cos_sim_function):
    '''This should be a function that returns the cosine similarity of the embeddings of a given 
    word from two different models. This is one way of looking at how the meaning of a word has 
    changed over time: taking word embeddings, for the same word, from two models trained on 
    different time periods’ text, and comparing their similarity.

    Parameters:
        word (str): The target word, whose embeddings from different models the function compute 
                    cosine similarity between. 
        model1 (gensim.models.word2vec.Word2Vec): The first Word2Vec model whose embedding of the 
                                                  target word will be compared. 
        model2 (gensim.models.word2vec.Word2Vec): The other Word2Vec model whose embedding of the 
                                                  target word will be compared.
        cos_sim_function (function): The function used to compute cosine similarity between two 
                                     vectors. Here, when actually run, this will be the cos_sim 
                                     function defined above.
    Returns:
        cosine_similarity_of_embeddings (float): Cosine similarity between the embeddings of the 
                                                 target word from the first and second Word2Vec model. 
    '''
    # get the embeddings of the word from the two models
    embedding1 = model1.wv[word]
    embedding2 = model2.wv[word]

    # calculate the cosine similarity between the two embeddings
    cosine_similarity_of_embeddings = cos_sim_function(embedding1, embedding2)
    return cosine_similarity_of_embeddings

def get_average_cos_sim(word, neighbors, model, cos_sim_function):
    '''This should be a function that returns the average of the cosine similarities between the 
    embeddings of a target word and the embeddings of a set of ‘neighborhood’ words. This is another 
    way of looking at how the meaning of a word has changed over time, without worrying about 
    alignment: by comparing how similar a word’s embedding is to those of different sets of words at 
    different points in time.
    
    Parameters:
        word (str): The target word, whose embedding the function compares with those of 
                    ‘neighborhood’ words.
        neighbors (List of str): A list of words whose embeddings the target word’s embedding will 
                                 be compared with.
        model (gensim.models.word2vec.Word2Vec): The Word2Vec model whose embeddings will be used.
        cos_sim_function (function): The function used to compute cosine similarity between two 
                                     vectors. Here, when actually run, this will be the cos_sim 
                                     function defined above.
    Returns:
        avg_cosine_similarity (float): Mean of the cosine similarities between the embedding of the 
                                       target word and the embedding of each of the ‘neighbor’ words.
    '''

    # get the embeddings of the word and the neighbors, skip if word is not found in model
    embeddings = [model.wv[neighbor] for neighbor in neighbors if neighbor in model.wv]

    # calculate the cosine similarity between the word and each neighbor
    cos_sims = [cos_sim_function(model.wv[word], embedding) for embedding in embeddings]

    # calculate the average cosine similarity
    avg_cosine_similarity = np.mean(cos_sims)

    return avg_cosine_similarity

''' ---------- PART 4 ---------- '''
def create_tfidf_matrix(documents, stopwords):
    '''Parameters:
        documents (list of nltk.corpus.reader.tagged.TaggedCorpusView): Collection of documents using NLTK corpuses
        stopwords (list of str): List of stopwords

    Returns:
        tfidf_matrix (np.ndarray): TF-IDF matrix of dimension (num_docs, num_words)
        vocab (list of str): List of vocabulary terms as inferred from the corpus
    '''

    # pre-process the document
    # 1. lowercasing words
    # 2. excluding stopwords
    # 3. including alphanumeric strings only (use isalnum).
    processed_docs = []
    for doc in documents:
        processed_doc = [word.lower() for word in doc if word.lower() not in stopwords and word.isalnum()]
        processed_docs.append(processed_doc)

    # get the vocabulary of the documents and sort them in alphabetical order
    vocab = sorted(set(word for doc in processed_docs for word in doc))

    # initialize the tfidf matrix
    tfidf_matrix = np.zeros((len(documents), len(vocab)))

    # get the number of documents
    N = len(documents)

    # get the number of documents in which each word appears
    nt = Counter()
    for doc in processed_docs:
        for word in set(doc):
            nt[word] += 1

    # calculate the tfidf matrix
    for i, doc in enumerate(processed_docs):
        # get the frequency of each word in the document
        f_td = Counter(doc)

        for j, word in enumerate(vocab):
            # calculate the tf value using f_t,d / sum(f_t',d)
            tf = f_td[word]

            # calculate the idf value using log10(N/1+nt) + 1
            idf = np.log10(N / (1 + nt[word])) + 1

            # calculate the tfidf value
            tfidf_matrix[i, j] = tf * idf

    return tfidf_matrix, vocab

def get_idf_values(documents, stopwords):
    # This part is ungraded, however, to test your code, you'll need to implement this function
    # If you have implemented create_tfidf_matrix, this implementation should be straightforward
    pass

def calculate_sparsity(tfidf_matrix):
    '''TF-IDF matrices are generally extremely sparse. In this function, given a TF-IDF matrix, 
    calculate the ratio between cells with value 0 and the total number of cells.
    
    Parameters:
        tfidf_matrix (np.ndarray): TF-IDF matrix as computed in the previous function
    Returns:
        sparsity (float): Sparsity of the matrix, detonated as a value between 0 and 1
    '''
    # calculate the sparsity of the matrix
    sparsity = np.count_nonzero(tfidf_matrix == 0) / tfidf_matrix.size
    return sparsity

def extract_salient_words(VT, vocabulary, k):
    '''It is possible to understand the hidden dimensions of the LSA model by computing words from 
    the vocabulary that are most associated with those dimensions. For each of the 10 latent 
    dimensions of LSA, find the top k most relevant terms in ascending order of relevance. 
    Dimensions are ordered as 0,1, 2, …. Hint: you only need VT and the vocabulary.
    
    Paramters:
        VT (np.ndarray): VT from SVD decomposition of TF-IDF matrix
        vocabulary (list of str): Vocabulary computed from the corpus
        k (int): Number of top salient words to return for each dimension
    Returns:
        salient_words (dict of {int: list of str}): Top-k salient words for each hidden dimension
    '''
    salient_words = {}

    # iterate over each dimension
    for i in range(VT.shape[0]):
        # get the indices of the top k salient words
        top_k_indices = np.argsort(VT[i])[-k:]

        # get the words corresponding to the indices
        top_k_words = [vocabulary[j] for j in top_k_indices]

        # store the top k words for the dimension
        salient_words[i] = top_k_words

    return salient_words

def get_similar_documents(U, Sigma, VT, doc_index, k):
    '''In this function, given a document index, return the indices of top k similar documents, 
    excluding the input document itself.

    Parameters:
        U (np.ndarray): U from SVD decomposition of TF-IDF matrix
        Sigma (np.ndarray): Sigma from SVD decomposition of TF-IDF matrix
        VT (np.ndarray): VT from SVD decomposition of TF-IDF matrix
        doc_index (int): Index of the document for which similar documents have to be retrieved. 
                         This refers to the doc_index row in the original TF-IDF matrix.
        k (int): Number of top similar documents to return

    Returns:
        similar_doc_indices (list of int): Indices of similar documents in the collection
    '''

    # calculate the document vector using U, Sigma, and VT
    # note: using broadcasting to do matrix multiplication
    doc_vector = U[doc_index] @ np.diag(Sigma) @ VT

    # calculate the cosine similarity between the document vector and all other document vectors
    cos_sims = np.array([cos_sim(doc_vector, U[i] @ np.diag(Sigma) @ VT) for i in range(U.shape[0])])

    # get the indices of the top k similar documents
    similar_doc_indices = list(np.argsort(cos_sims)[::-1][1:k+1]) # start at 1 to exclude input

    return similar_doc_indices


def document_retrieval(vocabulary, idf_values, U, Sigma, VT, query, k):
    '''LSA can be used to perform document retrieval. Given a text query, it can be viewed as a 
    mini-document, and then it can be compared with other documents in the corpus. Given a query, 
    first, create a TF-IDF representation of the query. Then project it into the low-dimensional LSA 
    semantic space. Finally, compute the similarity with all the documents in the corpus and return 
    the indices of top k. Refer to the Wikipedia page of LSA to know more about how to convert a query 
    into a mini-document. Although the query is treated as a mini-document, the vocabulary and IDF 
    values of the corpus do not change while evaluating a query. i.e. If a new word not present in 
    the vocabulary is seen in the query, it is ignored. The IDF values of the words in the 
    vocabulary do not change based on their occurrence statistics in the query.

    Parameters: 
        vocabulary (list of str): Vocabulary computed from corpus
        idf_values (dict of {str: float}): Words from vocabulary along with their IDF values
        U (np.ndarray): U from SVD decomposition of TF-IDF matrix
        Sigma (np.ndarray): Sigma from SVD decomposition of TF-IDF matrix
        VT (np.ndarray): VT from SVD decomposition of TF-IDF matrix
        query (list of str): Query for which top documents have to be retrieved
        k (int): Number of top documents indices to return

    Returns:
    retrieved_doc_indices (list of int): Indices of documents relevant to the query in decreasing 
                                         order of relevance score.
    '''
    
    # pre-process the query
    processed_query = [word.lower() for word in query if word.lower() in vocabulary]
    print(processed_query)

    # calculate the tf-idf representation of the query
    tfidf_query = np.zeros(len(vocabulary))
    for word in processed_query:
        tfidf_query[vocabulary.index(word)] += 1
    
    # divide by length of query
    tfidf_query /= len(processed_query)

    # multiply by idf value for that word
    for i, word in enumerate(processed_query):
        tfidf_query[i] *= idf_values[word]

    # project the query into the LSA space
    query_vector = np.dot(query, VT.T)

    # calculate the cosine similarity between the query vector and all document vectors
    cos_sims = np.array([cos_sim(query_vector, U[i] @ np.diag(Sigma) @ VT) for i in range(U.shape[0])])

    # get the indices of the top k similar documents
    retrieved_doc_indices = list(np.argsort(cos_sims)[::-1][:k])
    print(retrieved_doc_indices)
    return retrieved_doc_indices

if __name__ == '__main__':
    
    tweets = []
    with open('data/covid-tweets-2020-08-10-2020-08-21.tokenized.txt') as f:
        tweets = [line.strip() for line in f.readlines()]

    stop_words = []
    with open('data/stop_words.txt') as f:
        stop_words = [line.strip() for line in f.readlines()]


    """Building Vector Space model using PMI"""

    print(top_k_unigrams(tweets, stop_words, 10))
    # {'covid': 71281, 'pandemic': 50353, 'covid-19': 33591, 'people': 31850, 'n’t': 31053, 'like': 20837, 'mask': 20107, 'get': 19982, 'coronavirus': 19949, 'trump': 19223}
    frequent_unigrams = top_k_unigrams(tweets, stop_words, 1000)
    unigram_counter = top_k_unigrams(tweets, stop_words, -1)
    
    ### THIS PART IS JUST TO PROVIDE A REFERENCE OUTPUT
    sample_output = context_word_frequencies(tweets, stop_words, 2, frequent_unigrams)
    print(sample_output.most_common(10))
    """
    [(('the', 'pandemic'), 19811),
    (('a', 'pandemic'), 16615),
    (('a', 'mask'), 14353),
    (('a', 'wear'), 11017),
    (('wear', 'mask'), 10628),
    (('mask', 'wear'), 10628),
    (('do', 'n’t'), 10237),
    (('during', 'pandemic'), 8127),
    (('the', 'covid'), 7630),
    (('to', 'go'), 7527)]
    """
    ### END OF REFERENCE OUTPUT
    
    context_counter = context_word_frequencies(tweets, stop_words, 3, frequent_unigrams)

    word_vector = build_word_vector('ventilator', frequent_unigrams, unigram_counter, context_counter)
    print(get_top_k_dimensions(word_vector, 10))
    # {'put': 6.301874856316369, 'patient': 6.222687002250096, 'tried': 6.158108051673095, 'wearing': 5.2564459708663875, 'needed': 5.247669358807432, 'spent': 5.230966480014661, 'enjoy': 5.177980198384708, 'weeks': 5.124941187737894, 'avoid': 5.107686157639801, 'governors': 5.103879572210065}

    word_vector = build_word_vector('mask', frequent_unigrams, unigram_counter, context_counter)
    print(get_top_k_dimensions(word_vector, 10))
    # {'wear': 7.278203356425305, 'wearing': 6.760722107602916, 'mandate': 6.505074539073231, 'wash': 5.620700962265705, 'n95': 5.600353617179614, 'distance': 5.599542578641884, 'face': 5.335677912801717, 'anti': 4.9734651502193366, 'damn': 4.970725788331299, 'outside': 4.4802694058646}

    word_vector = build_word_vector('distancing', frequent_unigrams, unigram_counter, context_counter)
    print(get_top_k_dimensions(word_vector, 10))
    # {'social': 8.637723567642842, 'guidelines': 6.244375965192868, 'masks': 6.055876420939214, 'rules': 5.786665161219354, 'measures': 5.528168931193456, 'wearing': 5.347796214635814, 'required': 4.896659865603407, 'hand': 4.813598338358183, 'following': 4.633301876715461, 'lack': 4.531964710683777}

    word_vector = build_word_vector('trump', frequent_unigrams, unigram_counter, context_counter)
    print(get_top_k_dimensions(word_vector, 10))
    # {'donald': 7.363071158640809, 'administration': 6.160023745590209, 'president': 5.353905139926054, 'blame': 4.838868198365827, 'fault': 4.833928177006809, 'calls': 4.685281547339574, 'gop': 4.603457978983295, 'failed': 4.532989597142956, 'orders': 4.464073158650432, 'campaign': 4.3804665561680824}

    word_vector = build_word_vector('pandemic', frequent_unigrams, unigram_counter, context_counter)
    print(get_top_k_dimensions(word_vector, 10))
    # {'global': 5.601489175269805, 'middle': 5.565259949326977, 'amid': 5.241312533124981, 'handling': 4.609483077248557, 'ended': 4.58867551721951, 'deadly': 4.371399989758025, 'response': 4.138827482426898, 'beginning': 4.116495953781218, 'pre': 4.043655804452211, 'survive': 3.8777495603541254}

    word1_vector = build_word_vector('ventilator', frequent_unigrams, unigram_counter, context_counter)
    word2_vector = build_word_vector('covid-19', frequent_unigrams, unigram_counter, context_counter)
    print(get_cosine_similarity(word1_vector, word2_vector))
    # 0.2341567704935342

    word2_vector = build_word_vector('mask', frequent_unigrams, unigram_counter, context_counter)
    print(get_cosine_similarity(word1_vector, word2_vector))
    # 0.05127326904936171

    word1_vector = build_word_vector('president', frequent_unigrams, unigram_counter, context_counter)
    word2_vector = build_word_vector('trump', frequent_unigrams, unigram_counter, context_counter)
    print(get_cosine_similarity(word1_vector, word2_vector))
    # 0.7052644362543867

    word2_vector = build_word_vector('biden', frequent_unigrams, unigram_counter, context_counter)
    print(get_cosine_similarity(word1_vector, word2_vector))
    # 0.6144272810573133

    word1_vector = build_word_vector('trudeau', frequent_unigrams, unigram_counter, context_counter)
    word2_vector = build_word_vector('trump', frequent_unigrams, unigram_counter, context_counter)
    print(get_cosine_similarity(word1_vector, word2_vector))
    # 0.37083874436657593

    word2_vector = build_word_vector('biden', frequent_unigrams, unigram_counter, context_counter)
    print(get_cosine_similarity(word1_vector, word2_vector))
    # 0.34568665086152817


    """Exploring Word2Vec"""

    EMBEDDING_FILE = 'data/GoogleNews-vectors-negative300.bin.gz'
    word2vec = KeyedVectors.load_word2vec_format(EMBEDDING_FILE, binary=True)

    similar_words =  get_most_similar(word2vec, 'ventilator', 3)
    print(similar_words)
    # [('respirator', 0.7864563465118408), ('mechanical_ventilator', 0.7063839435577393), ('intensive_care', 0.6809945702552795)]

    # Word analogy - Tokyo is to Japan as Paris is to what?
    print(word_analogy(word2vec, 'Tokyo', 'Japan', 'Paris'))
    # ('France', 0.7889978885650635)


    """Word2Vec for Meaning Change"""

    # Comparing 40-60 year olds in the 1910s and 40-60 year olds in the 2000s
    model_t1 = Word2Vec.load('data/1910s_50yos.model')
    model_t2 = Word2Vec.load('data/2000s_50yos.model')

    # Cosine similarity function for vector inputs
    vector_1 = np.array([1,2,3,4])
    vector_2 = np.array([3,5,4,2])
    cos_similarity = cos_sim(vector_1, vector_2)
    print(cos_similarity)
    # 0.8198915917499229

    # Similarity between embeddings of the same word from different times
    major_cos_similarity = get_cos_sim_different_models("major", model_t1, model_t2, cos_sim)
    print(major_cos_similarity)
    # 0.19302374124526978

    # Average cosine similarity to neighborhood of words
    neighbors_old = ['brigadier', 'colonel', 'lieutenant', 'brevet', 'outrank']
    neighbors_new = ['significant', 'key', 'big', 'biggest', 'huge']
    print(get_average_cos_sim("major", neighbors_old, model_t1, cos_sim))
    # 0.6957747220993042
    print(get_average_cos_sim("major", neighbors_new, model_t1, cos_sim))
    # 0.27042335271835327
    print(get_average_cos_sim("major", neighbors_old, model_t2, cos_sim))
    # 0.2626224756240845
    print(get_average_cos_sim("major", neighbors_new, model_t2, cos_sim))
    # 0.6279034614562988

    ### The takeaway -- When comparing word embeddings from 40-60 year olds in the 1910s and 2000s,
    ###                 (i) cosine similarity to the neighborhood of words related to military ranks goes down;
    ###                 (ii) cosine similarity to the neighborhood of words related to significance goes up.


    """Latent Semantic Analysis"""

    import nltk
    nltk.download('brown')
    from nltk.corpus import brown
    documents = [brown.words(fileid) for fileid in brown.fileids()]

    # Exploring the corpus
    print("The news section of the Brown corpus contains {} documents.".format(len(documents)))
    for i in range(3):
        document = documents[i]
        print("Document {} has {} words: {}".format(i, len(document), document))
    # The news section of the Brown corpus contains 500 documents.
    # Document 0 has 2242 words: ['The', 'Fulton', 'County', 'Grand', 'Jury', 'said', ...]
    # Document 1 has 2277 words: ['Austin', ',', 'Texas', '--', 'Committee', 'approval', ...]
    # Document 2 has 2275 words: ['Several', 'defendants', 'in', 'the', 'Summerdale', ...]

    nltk.download('stopwords')
    from nltk.corpus import stopwords
    stopwords_list = stopwords.words('english')

    # This will take a few minutes to run
    tfidf_matrix, vocabulary = create_tfidf_matrix(documents, stopwords_list)
    idf_values = get_idf_values(documents, stopwords_list)

    print(tfidf_matrix.shape)
    # (500, 40881)

    print(tfidf_matrix[np.nonzero(tfidf_matrix)][:5])
    # [5.96857651 2.1079054  3.         2.07572071 2.69897   ]

    print(vocabulary[2000:2010])
    # ['amoral', 'amorality', 'amorist', 'amorous', 'amorphous', 'amorphously', 'amortization', 'amortize', 'amory', 'amos']

    print(calculate_sparsity(tfidf_matrix))
    # 0.9845266994447298

    """SVD"""
    U, Sigma, VT = randomized_svd(tfidf_matrix, n_components=10, n_iter=100, random_state=42)

    salient_words = extract_salient_words(VT, vocabulary, 10)
    print(salient_words[1])
    # ['anode', 'space', 'theorem', 'v', 'q', 'c', 'p', 'operator', 'polynomial', 'af']

    print("We will fetch documents similar to document {} - {}...".format(3, ' '.join(documents[3][:50])))
    # We will fetch documents similar to document 3 - 
    # Oslo The most positive element to emerge from the Oslo meeting of North Atlantic Treaty Organization Foreign Ministers has been the freer , 
    # franker , and wider discussions , animated by much better mutual understanding than in past meetings . This has been a working session of an organization that...

    similar_doc_indices = get_similar_documents(U, Sigma, VT, 3, 5)
    for i in range(2):
        print("Document {} is similar to document 3 - {}...".format(similar_doc_indices[i], ' '.join(documents[similar_doc_indices[i]][:50])))
    # Document 61 is similar to document 3 - 
    # For a neutral Germany Soviets said to fear resurgence of German militarism to the editor of the New York Times : 
    # For the first time in history the entire world is dominated by two large , powerful nations armed with murderous nuclear weapons that make conventional warfare of the past...
    # Document 6 is similar to document 3 - 
    # Resentment welled up yesterday among Democratic district leaders and some county leaders at reports that Mayor Wagner had decided to seek a third term with Paul R. Screvane and Abraham D. Beame as running mates . 
    # At the same time reaction among anti-organization Democratic leaders and in the Liberal party... 
    
    query = ['Krim', 'attended', 'the', 'University', 'of', 'North', 'Carolina', 'to', 'follow', 'Thomas', 'Wolfe']
    print("We will fetch documents relevant to query - {}".format(' '.join(query)))
    relevant_doc_indices = document_retrieval(vocabulary, idf_values, U, Sigma, VT, query, 5)
    for i in range(2):
        print("Document {} is relevant to query - {}...".format(relevant_doc_indices[i], ' '.join(documents[relevant_doc_indices[i]][:50])))
    # type: ignore # Document 90 is relevant to query - 
    # One hundred years ago there existed in England the Association for the Promotion of the Unity of Christendom . 
    # Representing as it did the efforts of only unauthorized individuals of the Roman and Anglican Churches , and urging a communion of prayer unacceptable to Rome , this association produced little...
    # Document 101 is relevant to query - To what extent and in what ways did Christianity affect the United States of America in the nineteenth century ? ? 
    # How far and in what fashion did it modify the new nation which was emerging in the midst of the forces shaping the revolutionary age ? ? To what...
