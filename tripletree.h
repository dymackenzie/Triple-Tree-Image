/**
 * @file        tripletree.h
 *
 */

#ifndef _TRIPLETREE_H_
#define _TRIPLETREE_H_

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"

using namespace std;
using namespace cs221util;

/**
 * The Node class *should be* private to the tree class via the principle of
 * encapsulation---the end user does not need to know our node-based
 * implementation details.
 * Given for PA3, and made public for convenience of testing and debugging
 */
class Node {
public:
    pair<unsigned int, unsigned int> upperleft;	// upper-left coordinates of Node's subimage
    unsigned int width;	 // horizontal dimension of Node's subimage in pixels
    unsigned int height; // vertical dimension of Node's subimage in pixels
    RGBAPixel avg;       // Average color of Node's subimage
    Node* A;	         // ptr to left or upper subtree
    Node* B;	         // ptr to middle subtree
    Node* C;	         // ptr to right or lower subtree

    // Node constructors
    Node(pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
        upperleft = ul;
        width = w;
        height = h;
        avg = RGBAPixel();
        A = nullptr; B = nullptr; C = nullptr;
    }
};

class TripleTree {

public:

    /* =============== start of given functions ====================*/

    /**
     * TripleTree destructor.
     * Destroys all of the memory associated with the
     * current TripleTree. This function should ensure that
     * memory does not leak on destruction of a TripleTree.
     *
     * @see TripleTree.cpp
     */
    ~TripleTree();

    /**
     * Copy constructor for a TripleTree.
     * Since TripleTree allocate dynamic memory (i.e., they use "new", we
     * must define the Big Three). This uses your implementation
     * of the copy function.
     * @see TripleTree.cpp
     *
     * @param other - the TripleTree we are copying.
     */
    TripleTree(const TripleTree& other);

    /**
     * Overloaded assignment operator for TripleTree.
     * Part of the Big Three that we must define because the class
     * allocates dynamic memory. This uses your implementation
     * of the copy and clear funtions.
     *
     * @param rhs - the right hand side of the assignment statement.
     */
    TripleTree& operator=(const TripleTree& rhs);

    /* =============== end of given functions ====================*/

    /* =============== public PA3 FUNCTIONS =========================*/

    /**
     * Constructor that builds a TripleTree out of the given PNG.
     *
     * Every leaf in the constructed tree corresponds to a pixel in the PNG.
     *
     * @param imIn - the input image used to construct the tree
     */
    TripleTree(PNG& imIn);

    /**
     * Render returns a PNG image consisting of the pixels
     * stored in the tree. It may be used on pruned trees. Draws
     * every leaf node's rectangle onto a PNG canvas using the
     * average color stored in the node.
     */
    PNG Render() const;

    /**
     * Prune function trims subtrees as high as possible in the tree.
     * A subtree is pruned (cleared) if all of its leaves are within
     * tol of the average color stored in the root of the subtree.
     * Pruning criteria should be evaluated on the original tree, not
     * on a pruned subtree.
     *
     * 
     * @param tol - maximum allowable RGBA color distance to qualify for pruning
     */
    void Prune(double tol);

    /**
     * Rearranges the tree contents so that when rendered, the image appears
     * to be mirrored horizontally (flipped over a vertical axis).
     * This may be called on pruned trees and/or previously flipped/rotated trees.
     * 
     */
    void FlipHorizontal();

    /**
     * Rearranges the tree contents so that when rendered, the image appears
     * to be rotated 90 degrees counter-clockwise.
     * This may be called on pruned trees and/or previously flipped/rotated trees.
     */
    void RotateCCW();

    /**
     * Returns the number of leaf nodes in the tree.
     *
     */
    int NumLeaves() const;

    /* =============== end of public PA3 FUNCTIONS =========================*/

private:
    /**
     * Private member variables.
     */
    Node* root;	 // pointer to the root of the TripleTree

    /**
     * Destroys all dynamically allocated memory associated with the
     * current TripleTree object.
     */
    void Clear();

    /**
     * Copies the parameter other TripleTree into the current TripleTree.
     * Does not free any memory. Called by copy constructor and operator=.
     * @param other - The TripleTree to be copied.
     */
    void Copy(const TripleTree& other);

    /**
     * Private helper function for the constructor. Recursively builds
     * the tree according to the specification of the constructor.
     * @param im - reference image used for construction
     * @param ul - upper left point of node to be built's rectangle.
     * @param w - width of node to be built's rectangle.
     * @param h - height of node to be built's rectangle.
     */
    Node* BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h);

    // added helper functions for all the functions above
    RGBAPixel FindAverage(Node* a, Node* b, Node* c);
    RGBAPixel FindAverage(Node* a, Node* c);
    int leaves(Node* subroot) const;
    void renderHelper(PNG &img, Node *subRoot) const;
    void clearHelper(Node* subRoot);
    Node* CopyHelper(Node* other);
    Node* FlipHorizontalHelper(Node*& subRoot); 
    Node* MoveToRight(Node*& subRoot, unsigned int x); 
    Node* MoveToLeft(Node*& subRoot, unsigned int x); 
    Node* RotateCCWHelper(Node*& subRoot);
    Node* FlipVertical(Node*& subRoot);
    Node* MoveDown(Node*& subRoot, unsigned int y);
    Node* MoveUp(Node*& subRoot, unsigned int y);
    Node* FlipHorizontalHelperRotate(Node*& subRoot);
    Node* PruneHelper(Node* subRoot, double tol);
    bool ShouldPrune(Node* subRoot, RGBAPixel avg, double tol);

};

#endif