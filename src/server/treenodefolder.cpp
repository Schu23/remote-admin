#include "treenodefolder.h"
#include <Wt/WTreeNode>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
#include <string>
#include <time.h>

#include <Wt/WIconPair>
#include <Wt/WStringUtil>
#include <Wt/WText>
#include <Wt/WTree>
#include <Wt/WTreeNode>
#include <Wt/WCheckBox>

using namespace Wt;
//namespace fs = boost::filesystem;

/*
TreeNodeFolder::TreeNodeFolder(const WString &labelText, WIconPair *labelIcon=0, WTreeNode *parentNode=0)
    : WTreeNode(labelText, labelIcon, parentNode)
{

    fs::path full_path( fs::initial_path<fs::path>() );
    full_path = fs::system_complete( labelText.toUTF8() );

    fs::directory_iterator end_iter;
    for ( fs::directory_iterator dir_itr( full_path );
         dir_itr != end_iter;
         ++dir_itr )
        {

            if ( fs::is_directory( dir_itr->status() ) )
                {
                    //++dir_count;
                    std::cout << dir_itr->path().filename() << " [directory]\n";
                    this->addChildNode(new TreeNodeFolder(dir_itr->path().filename()));
                }




        }
}*/
TreeNodeFolder::TreeNodeFolder(const boost::filesystem::path& path, bool showHidden)
    : WTreeNode(path.leaf() ), _path(path)
{
    _showHidden = showHidden;
    if(showHidden)
    {
        _visible = true;
    }
    else
        _visible = this->computeVisibility();

    label()->setTextFormat(PlainText);

    this->addStyleClass("treenodefolder");
    this->setLoadPolicy(WTreeNode::NextLevelLoading);

    this->setNodeVisible(_visible);
}

void TreeNodeFolder::test(WCheckBox *b)
{
    std::vector<WTreeNode*>::const_iterator it;
    std::vector<WTreeNode*> v = this->childNodes();
    for(it = v.begin(); it != v.end(); it++)
    {
        std::cout << "ONE DELETED!\n";
        this->removeChildNode(*it);
        //delete *it;
    }
    _showHidden = b->isChecked();
    this->populate();
}

TreeNodeFolder::TreeNodeFolder(const boost::filesystem::path& path)
    : WTreeNode(path.leaf() /*, createIcon(path)*/), _path(path)
{



    TreeNodeFolder(path,false);







}
/**
  Check if is posible hide the node, and set this boolean
  */
bool TreeNodeFolder::computeVisibility()
{


    std::cout << "SSTritg:  " << _path.string() << std::endl;

    if ( (boost::starts_with(_path.string(), "/home") ||
          boost::starts_with(_path.string(), "/mnt")  ||
          _path.string().compare("/") == 0)          &&
         !boost::starts_with(_path.leaf(), ".") )
        _visible = true;
    else
        _visible = false;


    return _visible;

}

bool TreeNodeFolder::visible()
{
    return _visible;
}


void TreeNodeFolder::populate()
{
    try {
        //if (boost::filesystem::is_directory(path_)) {

        std::set<boost::filesystem::path> paths;
        boost::filesystem::directory_iterator end_itr;

        for (boost::filesystem::directory_iterator i(_path); i != end_itr; ++i)
            paths.insert(*i);

        for (std::set<boost::filesystem::path>::iterator i = paths.begin();
             i != paths.end(); ++i)
        {
            if (boost::filesystem::is_directory(*i))
            {
                TreeNodeFolder *temp = new TreeNodeFolder(*i,_showHidden);
                if(temp->visible())
                    addChildNode(temp);
            }
        }
        //}
    } catch (std::exception& e){//boost::filesystem::filesystem_error& e) {
        std::cout << " !!!!!!!!!!!!!!! " << e.what() << std::endl;
        //std::cerr << e.what() << std::endl;
    }
}

bool TreeNodeFolder::expandable()
{
    try {


        boost::filesystem::directory_iterator end_itr;
        for (boost::filesystem::directory_iterator i(_path); i != end_itr; ++i)
            if (boost::filesystem::is_directory(*i))
                return true;

    } catch (...){//boost::filesystem::filesystem_error& e) {
        std::cout << " !!!!!!!!!!!!!!! "  << std::endl;
        //std::cerr << e.what() << std::endl;
    }

    return false;

    if (!populated())
        return boost::filesystem::is_directory(_path);
    else
        return WTreeNode::expandable();
}

std::string TreeNodeFolder::getCompletePath()
{
    //std::string;

    return _path.string();

    WString path("");


    WTreeNode *root = tree()->treeRoot();
    WTreeNode *node = this;
    while(node != root)
    {
        path = "/" + node->label()->text() + path;
        node = node->parentNode();
    }


    return path.toUTF8();




}
