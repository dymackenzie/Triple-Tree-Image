/**
 * @file        tripletree.cpp
 *
 */

#include "tripletree.h"

 /**
      * Constructor that builds a TripleTree out of the given PNG.
      *
      * Every leaf in the constructed tree corresponds to a pixel in the PNG.
      *
      * @param imIn - the input image used to construct the tree
      */
TripleTree::TripleTree(PNG& imIn) {
	root = BuildNode(imIn, pair<unsigned int, unsigned int>(0, 0), imIn.width(), imIn.height());
}

/**
 * TripleTree destructor.
 * Destroys all of the memory associated with the
 * current TripleTree. This function should ensure that
 * memory does not leak on destruction of a TripleTree.
 *
 * @see TripleTree.cpp
 */
TripleTree::~TripleTree() {
	Clear();
}

/**
 * Copy constructor for a TripleTree.
 * Since TripleTree allocate dynamic memory (i.e., they use "new", we
 * must define the Big Three). This uses your implementation
 * of the copy function.
 * @see TripleTree.cpp
 *
 * @param other - the TripleTree we are copying.
 */
TripleTree::TripleTree(const TripleTree& other) {
	Copy(other);
}

/**
 * Overloaded assignment operator for TripleTree.
 * Part of the Big Three that we must define because the class
 * allocates dynamic memory. This uses your implementation
 * of the copy and clear funtions.
 *
 * @param rhs - the right hand side of the assignment statement.
 */
TripleTree& TripleTree::operator=(const TripleTree& rhs) {
	// only take action if this object is not living at the same address as rhs
	// i.e. this and rhs are physically different trees
	if (this != &rhs) {
		// release any previously existing memory associated with this tree
		Clear();
		root = nullptr;
		// and then copy the other tree
		Copy(rhs);
	}
	return *this;
}

/**
 * Render returns a PNG image consisting of the pixels
 * stored in the tree. It may be used on pruned trees. Draws
 * every leaf node's rectangle onto a PNG canvas using the
 * average color stored in the node.
 *
 * You may want a recursive helper function for this.
 */
PNG TripleTree::Render() const {
    // replace the line below with your implementation
    PNG png = PNG(root->width, root->height);
    renderHelper(png, root);
    return png;
}

/**
 * Prune function trims subtrees as high as possible in the tree.
 * A subtree is pruned (cleared) if all of its leaves are within
 * tol of the average color stored in the root of the subtree.
 * Pruning criteria should be evaluated on the original tree, not
 * on a pruned subtree. (we only expect that trees would be pruned once.)
 *
 * You may want a recursive helper function for this.
 *
 * @param tol - maximum allowable RGBA color distance to qualify for pruning
 */
void TripleTree::Prune(double tol) {
	root = PruneHelper(root, tol);
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be mirrored horizontally (flipped over a vertical axis).
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::FlipHorizontal() {
    FlipHorizontalHelper(root);
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be rotated 90 degrees counter-clockwise.
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::RotateCCW() {
    FlipHorizontalHelperRotate(root);
	RotateCCWHelper(root);
}

/**
 * Returns the number of leaf nodes in the tree.
 *
 * You may want a recursive helper function for this.
 */
int TripleTree::NumLeaves() const {
    return leaves(root);
}

/**
     * Destroys all dynamically allocated memory associated with the
     * current TripleTree object. To be completed for PA3.
     * You may want a recursive helper function for this one.
     */
void TripleTree::Clear() {
	clearHelper(root);
    root = nullptr;
}

/**
 * Copies the parameter other TripleTree into the current TripleTree.
 * Does not free any memory. Called by copy constructor and operator=.
 * You may want a recursive helper function for this one.
 * @param other - The TripleTree to be copied.
 */
void TripleTree::Copy(const TripleTree& other) {
	root = CopyHelper(other.root);
}

/**
 * Private helper function for the constructor. Recursively builds
 * the tree according to the specification of the constructor.
 * @param im - reference image used for construction
 * @param ul - upper left point of node to be built's rectangle.
 * @param w - width of node to be built's rectangle.
 * @param h - height of node to be built's rectangle.
 */
Node* TripleTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
    Node *root = new Node(pair<unsigned int, unsigned int>(ul.first, ul.second), w, h);
    Node *a, *b, *c;
    // base case
    if (w == 1 && h == 1) {
        root->avg = (*im.getPixel(ul.first, ul.second));
        return root;
    }
    // three case
    if (w >= h) {
        // two case first
        if (w == 2) {
            a = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second), w / 2, h);
            c = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + 1, ul.second), w / 2, h);
            root->avg = FindAverage(a, c);
            root->A = a;
            root->C = c;
            return root;
        }
        int split = w % 3;
        switch (split) {
            case 0:
                // equally divide
                a = BuildNode(im, ul, w / 3, h);
                b = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + (w / 3), ul.second), w / 3, h);
                c = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + (2 * (w / 3)), ul.second), w / 3, h);
                root->avg = FindAverage(a, b, c);
                root->A = a;
                root->B = b;
                root->C = c;
                return root;
            case 1:
                // B gets extra pixel
                a = BuildNode(im, ul, w / 3, h);
                b = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + (w / 3), ul.second), (w / 3) + 1, h);
                c = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + (2 * (w / 3)) + 1, ul.second), w / 3, h);
                root->avg = FindAverage(a, b, c);
                root->A = a;
                root->B = b;
                root->C = c;
                return root;
            case 2:
                // both A and C gets extra pixel
                a = BuildNode(im, ul, (w / 3) + 1, h);
                b = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + (w / 3) + 1, ul.second), (w / 3), h);
                c = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + (2 * (w / 3)) + 1, ul.second), (w / 3) + 1, h);
                root->avg = FindAverage(a, b, c);
                root->A = a;
                root->B = b;
                root->C = c;
                return root;
            default:
                return root;
        }
    } else {
        // two case first
        if (h == 2) {
            a = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second), w, h / 2);
            c = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + 1), w, h / 2);
            root->avg = FindAverage(a, c);
            root->A = a;
            root->C = c;
            return root;
        }
        int split = h % 3;
        switch (split) {
            case 0:
                // equally divide
                a = BuildNode(im, ul, w, h / 3);
                b = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + (h / 3)), w, h / 3);
                c = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + 2 * (h / 3)), w, h / 3);
                root->avg = FindAverage(a, b, c);
                root->A = a;
                root->B = b;
                root->C = c;
                return root;
            case 1:
                // B gets extra pixel
                a = BuildNode(im, ul, w, h / 3);
                b = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + (h / 3)), w, (h / 3) + 1);
                c = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + (2 * (h / 3)) + 1), w, h / 3);
                root->avg = FindAverage(a, b, c);
                root->A = a;
                root->B = b;
                root->C = c;
                return root;
            case 2:
                // both A and C gets extra pixel
                a = BuildNode(im, ul, w, (h / 3) + 1);
                b = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + (h / 3) + 1), w, (h / 3));
                c = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + (2 * (h / 3)) + 1), w, (h / 3) + 1);
                root->avg = FindAverage(a, b, c);
                root->A = a;
                root->B = b;
                root->C = c;
                return root;
            default:
                return root;
        }
    }
}

/**
 * Takes three nodes and returns the average of all three nodes (from the average of their leaf nodes
 * if nodes are not leaf nodes)
 * 
 * @param a - first node
 * @param b - second node
 * @param c - third node
 */
RGBAPixel TripleTree::FindAverage(Node *a, Node *b, Node *c) {
    RGBAPixel avg; 

    int a_pixels = a->width * a->height;
    int b_pixels = b->width * b->height;
    int c_pixels = c->width * c->height;

    int total_pixels = a_pixels + b_pixels + c_pixels;

    double avg_a = (a_pixels * a->avg.a + b_pixels * b->avg.a + c_pixels * c->avg.a) / total_pixels;
    int avg_r = (a_pixels * a->avg.r + b_pixels * b->avg.r + c_pixels * c->avg.r) / total_pixels;
    int avg_g = (a_pixels * a->avg.g + b_pixels * b->avg.g + c_pixels * c->avg.g) / total_pixels;
    int avg_b = (a_pixels * a->avg.b + b_pixels * b->avg.b + c_pixels * c->avg.b) / total_pixels;
    
    if (avg_a < 0) {
        avg.a = 0; 
    } else if (avg_a > 1) {
        avg.a = 1; 
    } else {
        avg.a = avg_a; 
    }

    if (avg_r < 0) {
        avg.r = 0; 
    } else if (avg_r > 255) {
        avg.r = 255; 
    } else {
        avg.r = avg_r; 
    }

    if (avg_g < 0) {
        avg.g = 0; 
    } else if (avg_g > 255) {
        avg.g = 255; 
    } else {
        avg.g = avg_g; 
    }

    if (avg_b < 0) {
        avg.b = 0; 
    } else if (avg_b > 255) {
        avg.b = 255; 
    } else {
        avg.b = avg_b; 
    }

    return avg; 
}

/**
 * Takes two nodes and returns the average of all two nodes (from the average of their leaf nodes
 * if nodes are not leaf nodes)
 * 
 * @param a - first node (based off triple tree structure)
 * @param c - third node (based off triple tree structure)
 */
RGBAPixel TripleTree::FindAverage(Node *a, Node *c) {
    RGBAPixel avg; 

    int a_pixels = a->width * a->height;
    int c_pixels = c->width * c->height;

    int total_pixels = a_pixels + c_pixels;

    double avg_a = (a_pixels * a->avg.a + c_pixels * c->avg.a) / total_pixels;
    int avg_r = (a_pixels * a->avg.r + c_pixels * c->avg.r) / total_pixels;
    int avg_g = (a_pixels * a->avg.g + c_pixels * c->avg.g) / total_pixels;
    int avg_b = (a_pixels * a->avg.b + c_pixels * c->avg.b) / total_pixels;
    
    if (avg_a < 0) {
        avg.a = 0; 
    } else if (avg_a > 1) {
        avg.a = 1; 
    } else {
        avg.a = avg_a; 
    }

    if (avg_r < 0) {
        avg.r = 0; 
    } else if (avg_r > 255) {
        avg.r = 255; 
    } else {
        avg.r = avg_r; 
    }

    if (avg_g < 0) {
        avg.g = 0; 
    } else if (avg_g > 255) {
        avg.g = 255; 
    } else {
        avg.g = avg_g; 
    }

    if (avg_b < 0) {
        avg.b = 0; 
    } else if (avg_b > 255) {
        avg.b = 255; 
    } else {
        avg.b = avg_b; 
    }

    return avg; 
}

/**
 * Helper function to calculate number of leaves in Triple Tree structure.
 * 
 * @param subRoot - root to count leaves
 */
int TripleTree::leaves(Node* subRoot) const {
    if (subRoot->A == nullptr && subRoot->B == nullptr && subRoot->C == nullptr) {
        return 1;
    } else {
        if (subRoot->B == nullptr) {
            return leaves(subRoot->A) + leaves(subRoot->C);
        } else {
            return leaves(subRoot->A) + leaves(subRoot->B) + leaves(subRoot->C);
        }
    }
}

/**
 * Helper function to render Triple Tree structure into a PNG.
 * 
 * @param img - reference to PNG structure
 * @param subRoot - pointer to node containing Triple Tree structure
 */
void TripleTree::renderHelper(PNG &img, Node *subRoot) const {
    if (subRoot->A == nullptr && subRoot->B == nullptr && subRoot->C == nullptr) {
        for (unsigned int y = 0; y < subRoot->height; y++) {
            for (unsigned int x = 0; x < subRoot->width; x++) {
                RGBAPixel *t = img.getPixel(subRoot->upperleft.first + x, subRoot->upperleft.second + y);
                *t = subRoot->avg;
            }
        }
    } else {
        renderHelper(img, subRoot->A);
        if (subRoot->B != nullptr) {
            renderHelper(img, subRoot->B);
        }
        renderHelper(img, subRoot->C);
    }
}

/**
 * Helper function to copy one Triple Tree structure to another
 * 
 * @param other - pointer to Node containing Triple Tree structure
 */
Node* TripleTree::CopyHelper(Node* other) {
    Node *root = new Node(pair<unsigned int, unsigned int>(other->upperleft.first, other->upperleft.second), other->width, other->height);
    root->avg = other->avg;
    if (other->A != nullptr)
        root->A = CopyHelper(other->A);
    if (other->B != nullptr) 
        root->B = CopyHelper(other->B);
    if (other->C != nullptr)
        root->C = CopyHelper(other->C);
    return root;
}

/**
 * Helper function to deallocate and delete Triple Tree structure.
 * 
 * @param subRoot - pointer to Node containing Triple Tree structure
 */
void TripleTree::clearHelper(Node* subRoot) {
    if (subRoot->A == nullptr && subRoot->B == nullptr && subRoot->C == nullptr) {
        delete subRoot;
    } else {
        clearHelper(subRoot->A);
        subRoot->A = nullptr;
        if (subRoot->B != nullptr) {
            clearHelper(subRoot->B);
            subRoot->B = nullptr;
        }
        clearHelper(subRoot->C);
        subRoot->C = nullptr;
        clearHelper(subRoot);
    }
}


Node* TripleTree::FlipHorizontalHelper(Node*& subRoot) {
    if (subRoot == NULL) {
        return NULL;
    }

    if (subRoot->A == NULL && subRoot->B == NULL && subRoot->C == NULL) {
        return subRoot;
    }

    if (subRoot->height > subRoot->width) {
        FlipHorizontalHelper(subRoot->A); 
        FlipHorizontalHelper(subRoot->B);
        FlipHorizontalHelper(subRoot->C);
        return subRoot; 
    } else { // width >= height
        FlipHorizontalHelper(subRoot->A); 
        FlipHorizontalHelper(subRoot->B);
        FlipHorizontalHelper(subRoot->C);

        unsigned int difference = abs((double) subRoot->A->upperleft.first - subRoot->C->upperleft.first);

        if (subRoot->A->upperleft.first > subRoot->C->upperleft.first) {
            MoveToLeft(subRoot->A, difference);
            MoveToRight(subRoot->C, difference);
        } else {
            MoveToRight(subRoot->A, difference);
            MoveToLeft(subRoot->C, difference);
        }

        swap(subRoot->A, subRoot->C); 
        return subRoot;
    }
}

Node* TripleTree::MoveToRight(Node*& subRoot, unsigned int x) {
    if (subRoot == NULL) {
        return NULL;
    }
    if (subRoot->A == NULL && subRoot->B == NULL && subRoot->C == NULL) {
        subRoot->upperleft.first += x;
        return subRoot;
    } else {
        subRoot->upperleft.first += x;
        MoveToRight(subRoot->A, x);
        MoveToRight(subRoot->B, x); 
        MoveToRight(subRoot->C, x); 
        return subRoot;
    }
}

// important: x must be <= subRoot->upperleft.first for subRoot and its children
Node* TripleTree::MoveToLeft(Node*& subRoot, unsigned int x) {
    if (subRoot == NULL) {
        return NULL;
    }
    if (subRoot->A == NULL && subRoot->B == NULL && subRoot->C == NULL) {
        subRoot->upperleft.first += -x;
        return subRoot;
    } else {
        subRoot->upperleft.first += -x;
        MoveToLeft(subRoot->A, x);
        MoveToLeft(subRoot->B, x); 
        MoveToLeft(subRoot->C, x);
        return subRoot;
    }
}

Node* TripleTree::RotateCCWHelper(Node*& subRoot) {
    if (subRoot == NULL) {
        return NULL;
    }
    if (subRoot->A == NULL && subRoot->B == NULL && subRoot->C == NULL) {
        swap(subRoot->upperleft.first, subRoot->upperleft.second);
        swap(subRoot->width, subRoot->height);
        return subRoot; 
    } else if (subRoot->width >= subRoot->height) { // wide or square case
        swap(subRoot->upperleft.first, subRoot->upperleft.second);
        swap(subRoot->width, subRoot->height);
        // swap(subRoot->A, subRoot->C); // new place
        RotateCCWHelper(subRoot->A);
        RotateCCWHelper(subRoot->B);
        RotateCCWHelper(subRoot->C); 
        swap(subRoot->A, subRoot->C); // original place 
    } else { // tall case
        swap(subRoot->upperleft.first, subRoot->upperleft.second);
        swap(subRoot->width, subRoot->height);
        RotateCCWHelper(subRoot->A);
        RotateCCWHelper(subRoot->B);
        RotateCCWHelper(subRoot->C);
    }
    return subRoot;
}

/**
 * Helper function to prune the Triple Tree structure.
 * 
 * @param subRoot - pointer to Node containing Triple Tree structure
 * @param tol - number corresponding to distanceTo function which will determine which nodes to prune
 */
Node* TripleTree::PruneHelper(Node* subRoot, double tol) {
    if (subRoot == nullptr) return nullptr;
    if (subRoot->A == nullptr && subRoot->B == nullptr && subRoot->C == nullptr) return subRoot;
    if (ShouldPrune(subRoot, subRoot->avg, tol)) {
        clearHelper(subRoot->A);
        subRoot->A = nullptr;
        if (subRoot->B != nullptr) {
            clearHelper(subRoot->B);
            subRoot->B = nullptr;
        }
        clearHelper(subRoot->C);
        subRoot->C = nullptr;
    } else {
        subRoot->A = PruneHelper(subRoot->A, tol);
        if (subRoot->B != nullptr)
            subRoot->B = PruneHelper(subRoot->B, tol);
        subRoot->C =PruneHelper(subRoot->C, tol);
    }
    return subRoot;
}

/**
 * Helper function to determine whether Node should be pruned based of tolerance
 * and leaves.
 * 
 * @param other - pointer to Node containing Triple Tree structure
 * @param avg - pixel to compare to
 * @param tol - number corresponding to distanceTo function which will determine which nodes to prune
 */
bool TripleTree::ShouldPrune(Node* subRoot, RGBAPixel avg, double tol) {
    if (subRoot->A == nullptr && subRoot->B == nullptr && subRoot->C == nullptr) {
        // leaf node
        if (subRoot->avg.distanceTo(avg) <= tol) {
            return true;
        } else {
            return false;
        }
    } else {
        // not leaf node
        if (subRoot->B != nullptr)
            return ShouldPrune(subRoot->A, avg, tol) && ShouldPrune(subRoot->B, avg, tol) && ShouldPrune(subRoot->C, avg, tol);
        else
            return ShouldPrune(subRoot->A, avg, tol) && ShouldPrune(subRoot->C, avg, tol);
    }
}

Node* TripleTree::FlipHorizontalHelperRotate(Node*& subRoot) {
    if (subRoot == NULL) {
        return NULL;
    }

    if (subRoot->A == NULL && subRoot->B == NULL && subRoot->C == NULL) {
        return subRoot;
    }

    if (subRoot->height > subRoot->width) {
        FlipHorizontalHelper(subRoot->A); 
        FlipHorizontalHelper(subRoot->B);
        FlipHorizontalHelper(subRoot->C);
        return subRoot; 
    } else { // width >= height
        FlipHorizontalHelper(subRoot->A); 
        FlipHorizontalHelper(subRoot->B);
        FlipHorizontalHelper(subRoot->C);

        unsigned int difference = abs((double) subRoot->A->upperleft.first - subRoot->C->upperleft.first);

        if (subRoot->A->upperleft.first > subRoot->C->upperleft.first) {
            MoveToLeft(subRoot->A, difference);
            MoveToRight(subRoot->C, difference);
        } else {
            MoveToRight(subRoot->A, difference);
            MoveToLeft(subRoot->C, difference);
        }
        return subRoot;
    }
}
