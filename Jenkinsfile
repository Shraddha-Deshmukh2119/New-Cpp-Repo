pipeline {
    agent { label 'windows' }

    environment {
        BUILD_DIR = 'build'
    }

    stages {
        stage('Configure') {
            steps {
                bat '''
                    set PATH=C:\\msys64\\mingw64\\bin;%PATH%
                    cmake -B %BUILD_DIR% -G Ninja ^
                        -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe ^
                        -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe ^
                        -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON -DBUILD_APPS=OFF
                '''
            }
        }

        stage('Build') {
            steps {
                bat 'cmake --build %BUILD_DIR%'
            }
        }

        stage('Test') {
            steps {
                bat 'ctest --test-dir %BUILD_DIR% --output-on-failure'
            }
        }

        stage('Coverage') {
            steps {
                bat '''
                    pip install gcovr
                    gcovr -r . --object-directory %BUILD_DIR% --filter Client/ --filter Server/ --xml %BUILD_DIR%\\coverage.xml
                '''
            }
        }
    }

    post {
        always {
            archiveArtifacts artifacts: 'build/coverage.xml', allowEmptyArchive: true
            junit allowEmptyResults: true, testResults: 'build/Testing/**/*.xml'
        }
    }
}
