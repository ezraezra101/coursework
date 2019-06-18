#Binary Search Trees:

#Finds the next element in the tree, sorted by size
def tree_successor(x)
	if x.right != nil
		return tree_minimum(x.right)
	end
	y = x.p
	
	while y != nil && x == y.right
		x = y
		y = y.p
	end
	
	return y
end

def tree_minimum(x)
	if x.left == nil
		return x
	else
		return tree_minimum(x.left)
	end
end


def tree_insert(tree, z)
	y = nil
	x = tree.root
	while x != nil #While nodes exist
		y = x
		if z.key < x.key #Move to left or right child
			x = x.left
		else
			x = x.right
		end
	end
	z.p = y
	if y == nil #If tree was empty
		tree.root = z
	elsif z.key < y.key #Put into left or right
		y.left = z
	else
		y.right = z
	end		
end


def tree_remove(tree, z)
	x = tree.root
	while x != nil && x.key != z.key
		if x.key > z.key
			x = x.left
		else
			x = x.right
		end
	end
	if x = nil
		return "Key not present"
	end
	tree_remove_me(tree, x);
end

def tree_remove_me(tree, z)
	if z.left == nil ==z.right
		replacement = nil
	elsif z.left == nil
		replacement = z.right
	elsif z.right == nil
		replacement = z.left
	else #Neither is nil
		replacement = tree_minimum(z.right)
		tree_remove_me(tree, replacement)
	end
	
	if z.p == nil
		tree.root = replacement
	elsif z.p.left == z
		z.p.left = replacement
	else
		z.p.right = replacement
	end
end



#Better tree_delete
def tree_delete(tree, z)
	if z.left == nil
		tree_transplant tree, z, z.right
	elsif z.right == nil
		transplant tree, z, z.left
	else
		y = tree_minimum z.right #Find successor
		if y.p != z
			transplant tree, y, y.right
			y.right = z.right
			y.right.p = p
		end
		transplant tree, z, y
		y.left = z.left
		y.left.p = y
	end
end		


#Replace subtree rooted at u with subtree rooted at v
# => So u's parent becomes v's parent
# => v becomes child of u's parent
# => 	U goes away...
def tree_transplant(tree, u, v)
	if u.p == nil
		tree.root = v
	elsif u == u.p.left
		u.p.left = v
	else
		u.p.right = v
	end
	
	if v != nil
		v.p = u.p
end