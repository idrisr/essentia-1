#include <essentia/algorithmfactory.h>
#include <essentia/streaming/streamingalgorithm.h>
#include <essentia/scheduler/network.h>
#include <essentia/streaming/algorithms/poolstorage.h>
#include <essentia/pool.h>

using namespace std;
using namespace essentia;
using namespace essentia::streaming;

int main(int argc,char** argv) {
    if (argc != 2) {
        cout << "Error: incorrect number of arguments." << endl;
        cout << "Usage: " << argv[0] << " audio_input " << endl;
        exit(1);
    }

    string audioFilename = argv[1];

    Pool pool;
    essentia::init();
    AlgorithmFactory &factory = streaming::AlgorithmFactory::instance();
    Algorithm *bpm = factory.create("PercivalBpmEstimator");

    float sampleRate = 48000.0;
    Algorithm* audio = factory.create("MonoLoader",
                                      "filename", audioFilename);

    audio->output("audio") >> bpm->input("signal");
    bpm->output("bpm") >> PC(pool, "bpm");
    essentia::scheduler::Network n(bpm);
    n.run();
    return 0;
}

