#ifndef BF_H
#define BF_H

#ifdef __cplusplus
extern "C" {
#endif

#define BF_BLOCK_SIZE 512    /* Το μέγεθος ενός block σε bytes */
#define BF_BUFFER_SIZE 250    /* Ο μέγιστος αριθμός block που κρατάμε στην μνήμη */
#define BF_MAX_OPEN_FILES 100 /* Ο μέγιστος αριθμός ανοικτών αρχείων */

typedef enum BF_ErrorCode {
  BF_OK,
  BF_OPEN_FILES_LIMIT_ERROR,     /* Υπάρχουν ήδη BF_MAX_OPEN_FILES αρχεία ανοικτά */
  BF_INVALID_FILE_ERROR,         /* Ο αναγνωριστικός αριθμός αρχείου δεν αντιστιχεί σε κάποιο ανοιχτό αρχείο */
  BF_ACTIVE_ERROR,               /* Το επίπεδο BF είναι ενεργό και δεν μπορεί να αρχικοποιηθεί */
  BF_FILE_ALREADY_EXISTS,        /* Το αρχείο δεν μπορεί να δημιουργιθεί γιατι υπάρχει ήδη */
  BF_FULL_MEMORY_ERROR,          /* Η μνήμη έχει γεμίσει με ενεργά block */
  BF_INVALID_BLOCK_NUMBER_ERROR, /* Το block που ζητήθηκε δεν υπάρχει στο αρχείο */
  BF_ERROR
} BF_ErrorCode;

typedef enum ReplacementAlgorithm {
  LRU,
  MRU
} ReplacementAlgorithm;


// Δομή Block
typedef struct BF_Block BF_Block;

/*
 * Η συνάρτηση BF_Block_Init αρχικοποιεί και δεσμεύει την κατάλληλη μνήμη
 * για την δομή BF_BLOCK.
 */
void BF_Block_Init(BF_Block **block);

/*
 * Η συνάρτηση BF_Block_Destroy αποδεσμεύει την μνήμη που καταλαμβάνει
 * η δομή BF_BLOCK.
 */
void BF_Block_Destroy(BF_Block **block);

/*
 * Η συνάρτηση BF_Block_SetDirty αλάζει την κατάσταση του block σε dirty.
 * Αυτό πρακτικά σημαίνει ότι τα δεδομένα του block έχουν αλλαχθεί και το
 * επίπεδο BF όταν χρειαστεί θα γράψει το block ξανά στον δίσκο. Σε
 * περίπτωση που απλός διαβάζουμε τα δεδομένα χωρίς να τα αλλάζουμε τότε
 * δεν χρειάζεται να καλέσουμε την συνάρτηση.
 */
void BF_Block_SetDirty(BF_Block *block);

/*
 * Η συνάρτηση BF_Βlock_GetData επιστρέφει ένα δείκτη στα δεδομένα του Block.
 * Άμα αλάξουμε τα δεδομένα θα πρέπει να κάνουμε το block dirty με την κλήση
 * της συνάρτησης BF_Block_GetData.
 */
char* BF_Block_GetData(const BF_Block *block);

/*
 * Με τη συνάρτηση BF_Init πραγματοποιείται η αρχικοποίηση του επιπέδου BF.
 * Μπορούμε να επιλέξουμε ανάμεσα σε δύο πολιτικές αντικατάστασις Block
 * εκείνης της LRU και εκείνης της MRU.
 */
BF_ErrorCode BF_Init(const ReplacementAlgorithm repl_alg);

/*
 * Η συνάρτηση BF_CreateFile δημιουργεί ένα αρχείο με όνομα filename το
 * οποίο αποτελείται από blocks. Αν το αρχείο υπάρχει ήδη τότε επιστρέφεται
 * κωδικός λάθους. Σε περίπτωση επιτυχούς εκτέλεσης της συνάρτησης επιστρέφεται
 * BF_OK, ενώ σε περίπτωση αποτυχίας επιστρέφεται κωδικός λάθους. Αν θέλετε να
 * δείτε το είδος του λάθους μπορείτε να καλέσετε τη συνάρτηση BF_PrintError.
 */
BF_ErrorCode BF_CreateFile(const char* filename);

/*
 * Η συνάρτηση BF_OpenFile ανοίγει ένα υπάρχον αρχείο από blocks με όνομα
 * filename και επιστρέφει το αναγνωριστικό του αρχείου στην μεταβλητή
 * file_desc. Σε περίπτωση επιτυχίας επιστρέφεται BF_OK ενώ σε περίπτωση
 * αποτυχίας, επιστρέφεται ένας κωδικός λάθους. Αν θέλετε να δείτε το είδος
 * του λάθους μπορείτε να καλέσετε τη συνάρτηση BF_PrintError.
 */
BF_ErrorCode BF_OpenFile(const char* filename, int *file_desc);

/*
 * Η συνάρτηση BF_CloseFile κλείνει το ανοιχτό αρχείο με αναγνωριστικό αριθμό
 * file_desc. Σε περίπτωση επιτυχίας επιστρέφεται BF_OK ενώ σε περίπτωση
 * αποτυχίας, επιστρέφεται ένας κωδικός λάθους. Αν θέλετε να δείτε το
 * είδος του λάθους μπορείτε να καλέσετε τη συνάρτηση BF_PrintError.
 */
BF_ErrorCode BF_CloseFile(const int file_desc);

/*
 * Η συνάρτηση Get_BlockCounter δέχεται ως όρισμα τον αναγνωριστικό αριθμό
 * file_desc ενός ανοιχτού αρχείου από block και βρίσκει τον αριθμό των
 * διαθέσιμων blocks του, τον οποίο και επιστρέφει στην μεταβλητή blocks_num.
 * Σε περίπτωση επιτυχίας επιστρέφεται BF_OK ενώ σε περίπτωση αποτυχίας,
 * επιστρέφεται ένας κωδικός λάθους. Αν θέλετε να δείτε το είδος του λάθους
 * μπορείτε να καλέσετε τη συνάρτηση BF_PrintError.
 */
BF_ErrorCode BF_GetBlockCounter(const int file_desc, int *blocks_num);

/*
 * Με τη συνάρτηση BF_AllocateBlock δεσμεύεται ένα καινούριο block για το
 * αρχείο με αναγνωριστικό αριθμό blockFile. Το νέο block δεσμεύεται πάντα
 * στο τέλος του αρχείου, οπότε ο αριθμός του block είναι
 * BF_getBlockCounter(...) - 1. Το block που δεσμεύεται καρφιτσώνεται
 * στην μνήμη (pin) και επιστρέφεται στην μεταβλητή block. Όταν δεν το
 * χρειαζόμαστε άλλο αυτό το block τότε πρέπει να ενημερώσουμε τον επίπεδο
 * block καλώντας την συνάρτηση BF_UnpinBlock. Σε περίπτωση επιτυχίας
 * επιστρέφεται BF_OK ενώ σε περίπτωση αποτυχίας, επιστρέφεται ένας κωδικός
 * λάθους. Αν θέλετε να δείτε το είδος του λάθους μπορείτε να καλέσετε τη
 * συνάρτηση BF_PrintError.
 */
BF_ErrorCode BF_AllocateBlock(const int file_desc, BF_Block *block);


/*
 * Η συνάρτηση BF_GetBlock βρίσκει το block με αριθμό block_num του ανοιχτού
 * αρχείου file_desc και το επιστρέφει στην μεταβλητή block. Το block που
 * δεσμεύεται καρφιτσώνεται στην μνήμη (pin). Όταν δεν χρειαζόμαστε άλλο αυτό
 * το block τότε πρέπει να ενημερώσουμε τον επίπεδο block καλώντας την συνάρτηση
 * BF_UnpinBlock. Σε περίπτωση επιτυχίας επιστρέφεται BF_OK ενώ σε περίπτωση
 * αποτυχίας, επιστρέφεται ένας κωδικός λάθους. Αν θέλετε να δείτε το είδος του
 * λάθους μπορείτε να καλέσετε τη συνάρτηση BF_PrintError.
 */
BF_ErrorCode BF_GetBlock(const int file_desc,
                         const int block_num,
                         BF_Block *block);

/*
 * Η συνάρτηση BF_UnpinBlock αποδεσμεύει το block από το επίπεδο Block το
 * οποίο κάποια στηγμή θα το γράψει στο δίσκο. Σε περίπτωση επιτυχίας
 * επιστρέφεται BF_OK ενώ σε περίπτωση αποτυχίας, επιστρέφεται ένας κωδικός
 * λάθους. Αν θέλετε να δείτε το είδος του λάθους μπορείτε να καλέσετε τη
 * συνάρτηση BF_PrintError.
 */
BF_ErrorCode BF_UnpinBlock(BF_Block *block);

/*
 * Η συνάρτηση BF_PrintError βοηθά στην εκτύπωση των σφαλμάτων που δύναται να
 * υπάρξουν με την κλήση συναρτήσεων του επιπέδου αρχείου block. Εκτυπώνεται
 * στο stderr μια περιγραφή του πιο σφάλματος.
 */
void BF_PrintError(BF_ErrorCode err);

/*
 * Η συνάρτηση BF_Close κλήνει το επίπεδο Block γράφοντας στον δίσκο όποια
 * block είχε στην μνήμη.
 */
void BF_Close();

#ifdef __cplusplus
}
#endif
#endif // BF_H
