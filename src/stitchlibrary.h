/*************************************************\
| Copyright (c) 2011 Stitch Works Software        |
| Brian C. Milco <brian@stitchworkssoftware.com>  |
\*************************************************/
#ifndef STITCHLIBRARY_H
#define STITCHLIBRARY_H

#include <QObject>
#include <QStringList>
#include <QMap>

class StitchSet;
class Stitch;

class QComboBox;

/**
 * The StitchLibrary is a set of StitchSets
 *
 * The Collection keeps track of all sets. Specifically the Collection
 * keeps track of the builtIn Set, and the masterSet. And knows about
 * temporary sets that are loaded when a file with custom stitches is
 * opened.
 *
 * The StitchLibrary acts as the point of reference for StitchSet
 * based operations such as findStitch();
 *
 * The master stitchSet is a link list to the stitches in their sets.
 */
class StitchLibrary : public QObject
{
    Q_OBJECT

public:
    static StitchLibrary* inst();
    ~StitchLibrary();

    //return the list of stitch sets.
    QList<StitchSet *> stitchSets() { return mStitchSets; }
    //return the master stitch set.
    StitchSet* masterStitchSet() { return mMasterSet; }
    StitchSet* builtIn() { return mBuiltIn; }
    
    //load all known stitch sets.
    void loadStitchSets();
    //create links to the stitches being used from each set/overlay
    void setupMasterSet();

    Stitch* findStitch(QString name);
    StitchSet* findStitchSet(QString setName);

    //fill in a dropdown list for selecting a stitch set.
    QStringList stitchSetList();
    //list all categories in all sets
    QStringList categoryList() const;
    //list all stitches in the master set by default, optionally show all stitches in all sets.
    QStringList stitchList(bool showAllSets = false) const;

    //creates a new set to the collection, and returns a pointer to it.
    StitchSet* createStitchSet(QString setName);
    //Remove and delete setName.
    void removeSet(QString setName);

    //add a stitch set from a file, and return a pointer to it.
    StitchSet* addStitchSet(QString fileName);

    bool masterHasStitch(Stitch *s);
    
    void addStitchToMasterSet(StitchSet *set, Stitch *s);
    /**
     * This function removes a stitch from the master set.
     * It can be called on stitches that aren't in the master set
     * as it checks them before it removes them.
     */
    void removeStitchFormMasterSet(Stitch *s);

    void resetMasterStitchSet();

    void saveAllSets();

private slots:
    void changeStitchName(QString setName, QString oldName, QString newName);
    
private:
    StitchLibrary();

    //generates the next file name that can be used for a stitch set.
    QString nextSetSaveFile();

    //loads the list of stitches and where the stitches are being pulled from.
    bool loadMasterList();
    void saveMasterList();
    
    static StitchLibrary* mInstance;

    QList<StitchSet*> mStitchSets;
    StitchSet* mMasterSet;
    StitchSet* mBuiltIn;

    QMap<QString, QString> mStitchList;
};

#endif //STITCHLIBRARY_H