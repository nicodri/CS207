

import unittest
import numpy as np

from binsearch import binary_search


class binary_search_test(unittest.TestCase):

    def test_use_case(self):
        mylist = [1, 2, 4, 5, 6, 7]
        self.assertEqual(binary_search(mylist, 1), 0)
        self.assertEqual(binary_search(mylist, 5), 3)
        self.assertEqual(binary_search(mylist, 7), 5)

    def test_special_value(self):
        self.assertEqual(binary_search([], 100), -1)
        self.assertEqual(binary_search([1, 2, np.inf], 2), 1)
        self.assertEqual(binary_search([1, 2, np.inf], np.inf), 2)
        with self.assertRaises(ValueError):
            self.assertEqual(binary_search([1, 2, 3], np.nan))
    
    def test_type(self):
        with self.assertRaises(TypeError):
            binary_search(['a', 'b', 'c'], 2)

    def test_not_inside(self):
        mylist = [1, 2, 4, 5, 6, 7]
        self.assertEqual(binary_search(mylist, 3), -1)
        self.assertEqual(binary_search(mylist, 0), -1)
        self.assertEqual(binary_search(mylist, 8), -1)

if __name__ == '__main__':
    unittest.main()