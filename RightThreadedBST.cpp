#include <iostream>
#include <string>
#include <cassert>

using namespace std;

template <typename KEY, typename T>
class RightThreadedBST {

	private:
		struct Elem {
		    KEY key;
		    T data;
		    Elem *left;
		    Elem *right;
		    bool rightThread; //normal right child link or a threadlink
		};
		Elem *mRoot; // a dummy root sentinel
		int mSize;
		
		
		// helper method for inserting record into tree.
		bool insert(Elem *&root, const KEY &key, const T &data, Elem *lastLeft) {
		
		    if (root == mRoot || root == 0 ) { //or root == NULL
		        root = new Elem;
		        root->key = key;
		        root->data = data;
		        root->left = 0;
		        root->right = lastLeft;
		        root->rightThread = true;
		        return true;
		    }
		
		    if (key == root->key) {// dont insert if Key already exists
		        return false;
		    }
		    // insert at left branch
		    if (key < root->key)
		        return insert( root->left, key, data, root);
		
		    // insert at right branch
		    if (! root->rightThread ) { // if right Thread is false
		        return insert(root->right, key, data, lastLeft);
		
		    }
		    else {  //if current elem is right threaded then add a new node
		        root->rightThread = false;
		        root->right = new Elem;
		        root->right->key = key;
		        root->right->data = data;
		        root->right->left = 0;
		        root->right->right = lastLeft;
		        root->right->rightThread = true;
		        return true;
		    }
		}
		
		// helper method for print tree
		void printTree(ostream& out, int level, Elem *p) const
		{
		    int i;
		    if (p) {
		        if (p->right && !p->rightThread)
		            printTree(out, level+1, p->right);
		        for(i=0;i<level;i++) {
		            out << "\t";
		        }
		        out << p->key << " " << p->data << '\n';
		        printTree(out, level+1, p->left);
		    }
		}
		
	public:
		
		RightThreadedBST() {
		    // create a dummy root node
		    mRoot = new Elem;
		    mRoot->left = mRoot;  // empty tree
		    mRoot->right = 0;
		    mRoot->rightThread = false;
		    mSize = 0;
		}
		
		// destructor
		~RightThreadedBST() {
		    if( !mSize ) //empty tree
		        delete mRoot;
		    else {
		        destructCode( mRoot->left );
		        delete mRoot;
		    }
		}

		Elem*& getRoot() {
			return mRoot;
		}
		// inserts an element into the Map; return true if successful
		bool insert(KEY key, T data)
		{
		    if (insert(mRoot->left, key, data, mRoot)) {
		        mSize++;
		        return true;
		    }
		    return false;
		}
		
		// common code for deallocation
		void destructCode(Elem *&p) {
		    if( p->left != NULL ) {
		        destructCode( p->left );
		    }
		    if( p->right != NULL && ( !p->rightThread ) ) {  // p is not right-threaded
		        destructCode( p->right );
		    }
		    delete p;
		}
		
		// return size of the Map
		int size() const{
		    return mSize;
		}
		
		// outputs information in tree in inorder traversal order
		ostream& print(ostream& out) const
		{
		    if (mRoot == mRoot->left)
		    {
		        return out;
		    }
		    printTree(out, 0, mRoot->left);   // print tree structure
		    return out;
		}
		
		Elem* find(KEY k) {
		    Elem* p = mRoot;
		    Elem* r = mRoot;
		    if(mSize) {
		        p = p->left;
		    }
		    else {
		        return r;
		    }
		    while(true) {
		        if( p->key == k ) {   // the key of p->left is ==  ( query key "k" from parameter )
		         	cout <<"Matched : " << k << endl;
				    break;
		        }
		        else {
		            if( k < p->key ) {
		                if( p->left == NULL ) {
		                    return r;
		                }
		                p = p->left;
		            }
		            else {
						if( p->right == NULL || (p->rightThread)) {
		                    return r;
		                }
		                p = p->right;
		            }
		        }
		    }
		    return p;
		}
};


int main() {
    RightThreadedBST<string, string> m1;
    cout << "\nRight Threaded Binary Tree: String Type Output:\n\n";
    assert( m1.insert("Ravi", "Shankar") == true);
    assert( m1.insert("India", "Bihar") == true);
    assert( m1.insert("Bihar", "Jehanabad") == true);
    assert( m1.insert("Indian", "Cricket") == true);
    assert( m1.insert("Tweet", "Feed") == true);
    m1.print(cout);
    cout << "Destruct Called:"<< endl;
	m1.find("Bihar");
	return 0;
}
