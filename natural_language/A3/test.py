import unittest
import code_1

class Test_Functions(unittest.TestCase):
    def test_top_k_unigrams(self):
        tweets = ["a man saw a cat", "man is playing with a dog", "we watched a cat movie"]
        stop_words = ["a", "is", "with"]
        expected_result = {'man': 2, 'cat': 2}
        result = code_1.top_k_unigrams(tweets, stop_words, 2)
        self.assertEqual(result, expected_result)

        tweets = ["let ’s go #football", "covid-19 #getvaccine"]
        stop_words = ["a", "is", "with", "’s"]
        expected_result = {'let': 1, 'go': 1, '#football': 1, 'covid-19': 1, '#getvaccine': 1}
        result = code_1.top_k_unigrams(tweets, stop_words, 5)
        self.assertEqual(result, expected_result)

    def test_context_word_frequencies(self):
        tweets = ["a man saw a cat", "man is playing with a dog", "we watched a cat movie"]
        stopwords = ["a", "is", "with"]
        frequent_unigrams = code_1.top_k_unigrams(tweets, stopwords, 4)
        expected = {'man': 2, 'cat': 2, 'saw': 1, 'playing': 1}
        self.assertEqual(frequent_unigrams, expected)
        
        frequent_unigrams = list(frequent_unigrams.keys())
        context_counter = code_1.context_word_frequencies(tweets, stopwords, 2, frequent_unigrams)
        expected = {('a', 'man'): 2, ('a', 'saw'): 2, ('man', 'saw'): 1, ('saw', 'man'): 1, ('saw', 'cat'): 1, ('a', 'cat'): 2, ('cat', 'saw'): 1, ('man', 'playing'): 1, ('is', 'man'): 1, ('is', 'playing'): 1, ('playing', 'man'): 1, ('with', 'playing'): 1, ('a', 'playing'): 1, ('watched', 'cat'): 1, ('movie', 'cat'): 1}
        self.assertEqual(context_counter, expected)

if __name__ == '__main__':
    unittest.main()