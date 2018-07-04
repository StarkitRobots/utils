pipeline {
    agent {
        dockerfile {
            additionalBuildArgs  '--build-arg SSH_KEY="`cat ~/.ssh/id_rsa`" --build-arg USER_ID=`id -u` --build-arg USER_GID=`id -g`'
        }
    }
    options { disableConcurrentBuilds() }

    environment {
        QN_SLACK_MSG = sh(returnStdout: true, script: '''
            echo "Build #$BUILD_NUMBER (<$JOB_DISPLAY_URL|Voir>)"
            echo "Branch: $BRANCH_NAME"
            echo "Auteur: `git show -s --pretty=%an`"
            echo "Message: `git show -s --pretty=%s`"
            ''')
        QN_COLOR_FAIL = '#7b0d1e'
        QN_COLOR_SUCCESS = '#44cf6c'
        QN_COLOR_NORMAL= '#439FE0'
    }

    stages {
        stage('prepare') {
            steps {
                slackSend color: "$QN_COLOR_NORMAL", message: "Démarrage du build\n\n$QN_SLACK_MSG"
                sh '''
                    cd ws
                    ./workspace setup
                    ./workspace install rhoban/utils
                    ./workspace build
                    ./workspace build_tests
                '''
            }
            post {
                failure {
                    slackSend color: "$QN_COLOR_FAIL", message: "Echec `prepare`\n\n$QN_SLACK_MSG"
                }
            }
        }

        stage('tests') {
            steps {
                sh '''
                    cd ws
                    ./workspace run_tests
                    ./workspace result_tests
                '''
            }

            post {
                always {
                    junit 'ws/build_release/**/test_results/**/*.xml'
                }
                success {
                    slackSend color: "$QN_COLOR_SUCCESS", message: "Tests validés\n\n$QN_SLACK_MSG"
                }
                failure {
                    slackSend color: "$QN_COLOR_FAIL", message: "Echec `tests`: Oh la la :grin:\n\n$QN_SLACK_MSG"
                }
            }
        }

        stage('generate docs') {
            steps {
                sh '''
                    cd ws
                    doxygen
                '''
            }

            post {
                success {
                    slackSend color: "$QN_COLOR_SUCCESS", message: "Docs générées\n\n$QN_SLACK_MSG"
                }
                failure {
                    slackSend color: "$QN_COLOR_FAIL", message: "Echec `generate docs`: Oh la la :grin:\n\n$QN_SLACK_MSG"
                }
            }
        }

        stage('collect docs') {
            when { branch 'master' }
            steps {
                sh '''
                    cd ws
                    tar czvf doc.tar.gz doc/html
                '''
            }

            post {
                success {
                    archive 'ws/doc.tar.gz'
                    slackSend color: "$QN_COLOR_SUCCESS", message: "Docs collectées\n\n$QN_SLACK_MSG"
                }
                failure {
                    slackSend color: "$QN_COLOR_FAIL", message: "Echec `collect docs`: Oh la la :grin:\n\n$QN_SLACK_MSG"
                }
            }
        }
    }
    post {
        always {
            deleteDir()
        }
    }
}
